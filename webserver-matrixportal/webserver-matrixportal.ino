#include <WebServer.h>
#include "WwwServer.h"
#include "config.h"
#include "mbedtls/base64.h"

WwwServer wwwServer;

// For POST of full frame
static const size_t WIDTH = 64;
static const size_t HEIGHT = 32;
static const size_t ELEMENT_COUNT = WIDTH * HEIGHT;
static const size_t EXPECTED_BYTES = ELEMENT_COUNT * sizeof(uint16_t);

uint16_t frame[HEIGHT][WIDTH];

void handleRoot()
{
  char temp[500];
  int sec = millis() / 1000;
  int hr = sec / 3600;
  int min = (sec / 60) % 60;
  sec = sec % 60;

  snprintf(
    temp, 500,

    "<h1>Hello from ESP32!</h1>"
    "<p>Uptime: %02d:%02d:%02d</p>"
    "<p>Status: <a href=\"/status\">Status</a></p>",

    hr, min, sec
  );

  wwwServer.sendHtmlHeader();
  wwwServer.sendContent(temp);
  wwwServer.sendHtmlFooter();
}

void handleStatus()
{
/*
  String json = "{";
  json += "\"requestCount\":" + String(requestCount) + ",";
  json += "\"freeHeap\":" + String(ESP.getFreeHeap()) + ",";
  json += "\"uptime\":" + String(millis()) + ",";
  json += "\"cpuFreq\":" + String(ESP.getCpuFreqMHz()) + ",";
  json += "\"chipModel\":\"" + String(ESP.getChipModel()) + "\",";
  json += "\"flashSize\":" + String(ESP.getFlashChipSize()) + ",";
  json += "\"wifiRSSI\":" + String(WiFi.RSSI());
  json += "}";
  
  server.send(200, "application/json", json);
*/

  String html = "<!-- body -->";
  html += "<h1>Status</h1>";
  html += "<table border='0'>";
  html += "<tr><td>Local IP</td><td>" + wwwServer.localIP() + "</td></tr>";
  html += "<tr><td>Free Heap</td><td>" + String(ESP.getFreeHeap()) + "</td></tr>";
  html += "<tr><td>Uptime</td><td>" + String(millis()) + "</td></tr>";
  html += "<tr><td>CPU Freq</td><td>" + String(ESP.getCpuFreqMHz()) + " MHz</td></tr>";
  html += "<tr><td>Chip Model</td><td>" + String(ESP.getChipModel()) + "</td></tr>";
  html += "<tr><td>Flash Size</td><td>" + String(ESP.getFlashChipSize()) + "</td></tr>";
  html += "</table>";
  html += "<p>Home: <a href=\"/\">Home</a></p>";

  wwwServer.sendHtmlHeader();
  wwwServer.sendContent(html);
  wwwServer.sendHtmlFooter();
}

void handleFrame()
{
    Serial.println("handleFrame");
    if (wwwServer.method() != HTTP_POST)
    {
        wwwServer.send(405, "text/plain", "Method Not Allowed");
        return;
    }

    Serial.println("Method OK");

    if (!wwwServer.hasArg("data"))
    {
        wwwServer.send(400, "text/plain", "Missing data field");
        return;
    }

    Serial.println("'data' field found");

    String b64 = wwwServer.arg("data");
    size_t len = b64.length();

    Serial.print("'data' length:");
    Serial.println(len);

    size_t required_size;
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
        return;
    }

    Serial.println("length OK");

    Serial.println("Method: " + String(wwwServer.method()));
    Serial.println("Content-Length: " + String(wwwServer.contentLength()));
    Serial.println("Args: " + String(wwwServer.args()));
    Serial.println("Arg[0]: " + wwwServer.argName(0));

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
        return;
    }

    // ✅ Now frame[][] is directly usable
    Serial.println(frame[0][0]);
    Serial.println(frame[31][63]);

    wwwServer.send(200, "text/plain", "OK");
}

void setup(void)
{
  Serial.begin(9600);
  delay(500);

  if (!wwwServer.init(SSID, PW))
  {
     Serial.println("HTTP server NOT started");
     Serial.println("Is the SSID and password correct ?");
     return;
  }

  wwwServer.on("/", handleRoot);
  wwwServer.on("/status", handleStatus);
  wwwServer.on("/frame", HTTP_POST, handleFrame);
  wwwServer.begin();

  Serial.println("HTTP server started");
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
    Serial.println(WiFi.localIP());
  }
}
