#include <WebServer.h>
#include "WwwServer.h"
#include "config.h"
#include "mbedtls/base64.h"
#include <Adafruit_Protomatter.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/TomThumb.h>

uint8_t rgbPins[]  = {42, 41, 40, 38, 39, 37};
uint8_t addrPins[] = {45, 36, 48, 35, 21};
uint8_t clockPin   = 2;
uint8_t latchPin   = 47;
uint8_t oePin      = 14;

enum class Modes {
  Idle,
  Text,
  Frame
};

WwwServer wwwServer;
Modes mode = Modes::Idle;

// For POST of full frame
static const size_t WIDTH = 64;
static const size_t HEIGHT = 32;
static const size_t ELEMENT_COUNT = WIDTH * HEIGHT;
static const size_t EXPECTED_BYTES = ELEMENT_COUNT * sizeof(uint16_t);

uint16_t frame[HEIGHT][WIDTH];

Adafruit_Protomatter matrix(
  64,          // Matrix width in pixels
  4,           // Bit depth -- 6 here provides maximum color options
  1, rgbPins,  // # of matrix chains, array of 6 RGB pins for each
  4, addrPins, // # of address pins (height is inferred), array of pins
  clockPin, latchPin, oePin, // Other matrix control pins
  true);       // HERE IS THE MAGIC FOR DOUBLE-BUFFERING!

#define MAX_TEXT_SIZE 64
#define MAX_COLOR_SIZE 10

int16_t  textX1;        // Current text position (X)
int16_t  textY1;        // Current text position (Y)
int16_t  textMin1;      // Text pos. (X) when scrolled off left edge
char     scrollText[MAX_TEXT_SIZE];      // Buffer to hold scrolling message text
char     scrollColorText[MAX_COLOR_SIZE];      // Buffer to hold scrolling message text
uint16_t scrollColor;

void showAPinfo()
{
  Serial.println("Showing AP info");
  //matrix.setFont(&FreeSans9pt7b);
  matrix.setFont(&TomThumb);
  matrix.setTextWrap(false); // Allow text off edge matrix.setTextColor(0xFFFF); 

  matrix.setTextColor(matrix.color565(32, 32, 32));
  matrix.setCursor(1, 6);
  matrix.print("Wifi:");

  matrix.setTextColor(matrix.color565(64, 0, 64));
  matrix.setCursor(18, 6);
  matrix.print(SSID);

  matrix.setTextColor(matrix.color565(32, 32, 32));
  matrix.setCursor(1, 12);
  matrix.print("Pw:");

  matrix.setTextColor(matrix.color565(64, 64, 0));
  matrix.setCursor(18, 12);
  matrix.print(PW);

  matrix.setTextColor(matrix.color565(0, 0, 64));
  matrix.setCursor(1, 18);
  char temp[100];
  sprintf(temp, "http://%s", WiFi.softAPIP().toString().c_str());
  matrix.print(temp);

  matrix.show();
}

void initText(const char* text, uint16_t color)
{
  // Set up the scrolling message...
  if (text == NULL)
  {
    sprintf(scrollText, "Coderdojo Belgium %dx%d", matrix.width(), matrix.height()); 
  }
  else
  {
    strncpy(scrollText, text, MAX_TEXT_SIZE);
  }
  matrix.setFont(&FreeSansBold18pt7b); // Use nice bitmap font 
  matrix.setTextWrap(false); // Allow text off edge matrix.setTextColor(0xFFFF); 

  scrollColor = color;

  int16_t  x1, y1;
  uint16_t w, h;

  matrix.getTextBounds(scrollText, 0, 0, &x1, &y1, &w, &h); // How big is it?
  textMin1 = -w; // All text is off left edge when it reaches this point
  textX1 = matrix.width(); // Start off right edge
  textY1 = matrix.height() / 2 - (y1 + h / 2); // Center text vertically

  mode = Modes::Text;
}

void handleRoot()
{
  /*
  char temp[500];
  int sec = millis() / 1000;
  int hr = sec / 3600;
  int min = (sec / 60) % 60;
  sec = sec % 60;

  snprintf(
    temp, 500,

    "<p>Uptime: %02d:%02d:%02d</p>",

    hr, min, sec
  );
*/
  wwwServer.sendHtmlHeader();

  wwwServer.sendTitle("Matrixportal");
  //wwwServer.sendContent(temp);
  wwwServer.sendFormStyle();
  wwwServer.sendHtmlForm(String(scrollText), String(scrollColorText));
  wwwServer.sendFormPostScript();

  wwwServer.sendNavButton("Status", "/status");
  wwwServer.sendNavButton("Draw", "/draw");

  wwwServer.sendHtmlFooter();
}

void handleStatus()
{
  wwwServer.sendHtmlHeader();
  wwwServer.sendFormStyle();
  wwwServer.sendTitle("Status");
  wwwServer.sendStatusPage(String(scrollText), String(scrollColorText));
  wwwServer.sendNavButton("Home", "/");
  wwwServer.sendHtmlFooter();
}

void handleDrawPage()
{
  wwwServer.sendHtmlHeader();
  wwwServer.sendFormStyle();
  wwwServer.sendTitle("Draw");
  wwwServer.sendDrawPage();
  wwwServer.sendNavButton("Home", "/");
  wwwServer.sendHtmlFooter();
}

void handleText()
{
    if (wwwServer.method() != HTTP_POST)
    {
        wwwServer.send(405, "text/plain", "Method Not Allowed");
        return;
    }

    if (!wwwServer.hasArg("text"))
    {
        wwwServer.send(400, "text/plain", "Missing text field");
        return;
    }

    String text = wwwServer.arg("text");

    Serial.println(text);

    if (text.length() >= MAX_TEXT_SIZE)
    {
        wwwServer.send(400, "text/plain", "Text too long");
        return;
    }

    uint16_t color = scrollColor;
    if (wwwServer.hasArg("color"))
    {
      String colorStr = wwwServer.arg("color"); // "#ff0000"
      strncpy(scrollColorText, colorStr.c_str(), MAX_COLOR_SIZE);
      Serial.println(colorStr);
      int r = strtoul(colorStr.substring(1,3).c_str(), NULL, 16);
      int g = strtoul(colorStr.substring(3,5).c_str(), NULL, 16);
      int b = strtoul(colorStr.substring(5,7).c_str(), NULL, 16);
      color = matrix.color565(r,  g,  b);
    }

    initText(text.c_str(), color);
    
    wwwServer.send(200, "text/plain", "OK");
}

void handleFrame()
{
    if (wwwServer.method() != HTTP_POST)
    {
        wwwServer.send(405, "text/plain", "Method Not Allowed");
        return;
    }

    if (!wwwServer.hasArg("data"))
    {
        wwwServer.send(400, "text/plain", "Missing data field");
        Serial.println("Missing data field");
        return;
    }

    String b64 = wwwServer.arg("data");
    size_t len = b64.length();
    size_t required_size;

    // check if size will be correct after decoding
    mbedtls_base64_decode(
        NULL,
        0,
        &required_size,
        (const unsigned char*)b64.c_str(),
        len
    );

    if (required_size != EXPECTED_BYTES)
    {
        wwwServer.send(400, "text/plain", "Invalid size");
        Serial.println("Invalid Size");
        return;
    }

    size_t max_decoded = (len * 3) / 4;

    size_t output_len;
    int ret = mbedtls_base64_decode(
        (unsigned char*)frame,
        max_decoded,
        &output_len,
        (const unsigned char*)b64.c_str(),
        len
    );

    if (ret != 0 || output_len != ELEMENT_COUNT * sizeof(uint16_t))
    {
        wwwServer.send(400, "text/plain", "Invalid data");
        Serial.println("Invalid Data");
        return;
    }

    wwwServer.send(200, "text/plain", "OK");
    Serial.println("Frame OK");
    mode = Modes::Frame;
}

void setup(void)
{
  Serial.begin(9600);
  delay(500);
  delay(5000);

  // Initialize matrix...
  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  if(status != PROTOMATTER_OK)
  {
    // DO NOT CONTINUE if matrix setup encountered an error.
    for(;;);
  }

  Serial.println(SSID);
  Serial.println(PW);

  if (!wwwServer.init(SSID, PW, USE_AP))
  {
     Serial.println("HTTP server NOT started");
     Serial.println("Is the SSID and password correct ?");
     return;
  }

  wwwServer.on("/", handleRoot);
  wwwServer.on("/status", handleStatus);
  wwwServer.on("/draw", handleDrawPage);
  wwwServer.on("/text", handleText);
  wwwServer.on("/frame", HTTP_POST, handleFrame);
  wwwServer.begin();

  Serial.println("HTTP server started");

  if (USE_AP)
  {
    showAPinfo();
  }
  else
  {
    initText(wwwServer.localIP().c_str(), matrix.color565(228,  0,  0));
  }

  strncpy(scrollColorText, "#FF0000", MAX_COLOR_SIZE);
}

void loop()
{
  wwwServer.handleClient();
  delay(10);
  
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 30000)
  {
    lastPrint = millis();
    Serial.println("Status: Requests: " + String(wwwServer.requestCount) + 
                   ", Free Heap: " + String(ESP.getFreeHeap()));
    Serial.print("IP address: ");
    if (USE_AP)
    {
      Serial.println(WiFi.softAPIP());
    }
    else
    {
      Serial.println(WiFi.localIP());
    }
  }

  if (mode == Modes::Text)
  {
    matrix.fillScreen(0); // Fill background black
  
    // Draw the big scrolly text
    matrix.setFont(&FreeSansBold18pt7b); // Use nice bitmap font
    matrix.setTextColor(scrollColor);
    matrix.setCursor(textX1, textY1);
    matrix.print(scrollText);
  
    // Update text position for next frame. If text goes off the
    // left edge, reset its position to be off the right edge.
    if ((--textX1) < textMin1) textX1 = matrix.width();
  
    matrix.show();
  }
  else if (mode == Modes::Frame)
  {
    Serial.println("Drawing frame");
    for (uint16_t y = 0; y < HEIGHT; y++)
    {
      for (uint16_t x = 0; x < WIDTH; x++)
      {
        matrix.drawPixel(x, y, frame[y][x]);
      }
    }
  
    matrix.show();

    mode = Modes::Idle;
  }
}
