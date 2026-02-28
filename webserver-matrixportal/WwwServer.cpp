#include <WiFi.h>
#include <NetworkClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "WwwServer.h"

WwwServer::WwwServer() : webServer(80)
{
}

bool WwwServer::init(const char *ssid, const char *password)
{
  char ssid_[100];
  char pw_[100];
  decode(ssid, ssid_, 100);
  decode(password, pw_, 100);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_, pw_);

  int waitCount = 0;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    waitCount++;
    if (waitCount > 60)
    {
        return false;
    }
  }

  /*
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  */
  if (MDNS.begin("esp32"))
  {
    //Serial.println("MDNS responder started");
  }

  webServer.onNotFound([this]() {
      this->handleNotFound();
  });

  //webServer.onNotFound(handleNotFound);

  started = true;
  return true;
}

void WwwServer::begin()
{
  if (started)
  {
    webServer.begin();
  }
}

String WwwServer::localIP()
{
    return WiFi.localIP().toString();
}

size_t WwwServer::contentLength()
{
  return webServer.clientContentLength();
}

void WwwServer::decode(const char *encoded, char *result, size_t result_size)
{          
   char *r = result;
   const char *e = encoded;
   uint16_t ix = 0;
   while (*e) { *r++ = *e++; e += ++ix; }
   *r = 0;
}  

void WwwServer::sendHtmlHeader()
{
  webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer.send(200, "text/html", "");
  webServer.sendContent(
    "<html>"
    "<head>"
    //"<meta http-equiv='refresh' content='10'/>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\"/>"
    "<title>ESP32 Demo</title>"
    "<style>"
    "body {"
    "  background-color: #181818;"
    "  font-family: Arial, Helvetica, Sans-Serif;"
    "  color: #FFFFFF;"
    "  padding: 20px;"
    "}"
    "</style>"
    "</head>"
    "<body>");
}

void WwwServer::sendHtmlForm(const String& currentText, const String& currentColor)
{
    webServer.sendContent(
      "<form id='myForm'>"

      "  <label for='textInput'>Text:</label>"
      "  <input type='text' id='textInput' name='text' value='" + htmlEscape(currentText) + "'>"

      "  <label for='colorInput'>Color:</label>"
      "  <input type='color' id='colorInput' name='color' value='" + currentColor + "'>"

      "  <button type='button' id='submitBtn'>Submit</button>"

      "</form>"
    );
}
void WwwServer::sendFormStyle()
{
    webServer.sendContent(
      "<style>"
      "form {"
      "  background-color: #282828;"
      "  padding: 20px;"
      "  border-radius: 8px;"
      "  max-width: 400px;"
      "  margin: auto;"
      "  box-shadow: 0 0 10px rgba(0,0,0,0.5);"
      "}"
      "label {"
      "  display: block;"
      "  margin-bottom: 5px;"
      "  font-weight: bold;"
      "}"
      "input[type='text'], input[type='color'] {"
      "  width: 100%;"
      "  padding: 8px;"
      "  margin-bottom: 15px;"
      "  border: 1px solid #555;"
      "  border-radius: 4px;"
      "  box-sizing: border-box;"
      "  background-color: #181818;"
      "  color: #fff;"
      "}"
      "button {"
      "  background-color: #4CAF50;"
      "  color: white;"
      "  padding: 10px 20px;"
      "  border: none;"
      "  border-radius: 4px;"
      "  cursor: pointer;"
      "  font-size: 16px;"
      "}"
      "button:hover {"
      "  background-color: #45a049;"
      "}"
      " .mobile-btn {"
      "   display:inline-block;"
      "   width:80%;"
      "   max-width:300px;"
      "   padding:16px;"
      "   font-size:18px;"
      "   font-weight:bold;"
      "   text-align:center;"
      "   color:#ffffff;"
      "   text-decoration:none;"
      "   border-radius:12px;"
      "   background-color:#4CAF50;"
      "   box-shadow:0 4px 8px rgba(0,0,0,0.4);"
      " }"
      ".page-title {"
      "   text-align:center;"
      "   margin:6px 0 18px 0;"
      "}"
      ".page-title h1 {"
      "   font-size:28px;"
      "   font-weight:bold;"
      "   margin:0;"
      "   padding-bottom:8px;"
      "   border-bottom:2px solid #4CAF50;"
      "   display:inline-block;"
      "}"
      "canvas {border:2px solid #4CAF50;background:black;touch-action:none; }"
      "</style>"
    );
}

void WwwServer::sendFormPostScript()
{
    webServer.sendContent(
      "<script>"
      "document.getElementById('submitBtn').addEventListener('click', function() {"
      "  const text = document.getElementById('textInput').value;"
      "  const color = document.getElementById('colorInput').value;"
      "  const data = new URLSearchParams();"
      "  data.append('text', text);"
      "  data.append('color', color);"
      "  fetch('/text', {"
      "    method: 'POST',"
      "    body: data,"
      "    headers: {'Content-Type': 'application/x-www-form-urlencoded'}"
      "  })"
      "  .then(response => response.text())"
      "  .then(result => console.log('Server response:', result))"
      "  .catch(error => console.error('Error:', error));"
      "});"
      "</script>"
    );
}

void WwwServer::sendHtmlFooter()
{
  webServer.sendContent("</body></html>");
  // End the reponse
  webServer.sendContent("");
}

void WwwServer::handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += webServer.uri();
  message += "\nMethod: ";
  message += (webServer.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += webServer.args();
  message += "\n";

  for (uint8_t i = 0; i < webServer.args(); i++)
  {
    message += " " + webServer.argName(i) + ": " + webServer.arg(i) + "\n";
  }

  webServer.send(404, "text/plain", message);

  requestCount++;
}

void WwwServer::sendStatusPage(const String& currentText, const String& currentColor)
{
    char temp[20];
    int sec = millis() / 1000;
    int hr = sec / 3600;
    int min = (sec / 60) % 60;
    sec = sec % 60;
  
    snprintf(
      temp, 20,  
      "%02d:%02d:%02d",
      hr, min, sec
    );

    // Start a container for the status info
    webServer.sendContent(
      "<div style='"
      "  background-color:#282828;"
      "  padding:20px;"
      "  border-radius:8px;"
      "  max-width:400px;"
      "  margin:auto;"
      "  color:#fff;"
      "  font-family:Arial,Helvetica,sans-serif;"
      "  box-shadow:0 0 10px rgba(0,0,0,0.5);"
      "'>"
    );

    // CPU info
    webServer.sendContent("<p><b>CPU Model:</b> " + String(ESP.getChipModel()) + "</p>");
    webServer.sendContent("<p><b>CPU Speed:</b> " + String(ESP.getCpuFreqMHz()) + " MHz</p>");
    webServer.sendContent("<p><b>Flash Size:</b> " + String(ESP.getFlashChipSize()) + " bytes</p>");
    webServer.sendContent("<p><b>Local IP:</b> " + localIP()  + "</p>");
    webServer.sendContent("<p><b>Uptime::</b> " + String(temp)  + "</p>");

    // Free memory
    webServer.sendContent("<p><b>Free Heap:</b> " + String(ESP.getFreeHeap()) + " bytes</p>");

    // Current text and color
    webServer.sendContent("<p><b>Current Text:</b> " + htmlEscape(currentText) + "</p>");
    webServer.sendContent("<p><b>Current Color:</b> <span style='color:" + currentColor + "'>" + currentColor + "</span></p>");

    // Optionally show a color swatch
    webServer.sendContent(
      "<div style='width:50px;height:50px;background-color:" + currentColor + 
      ";border:1px solid #fff;border-radius:4px;margin-top:5px;'></div>"
    );

    // End container
    webServer.sendContent("</div>");
}

void WwwServer::sendNavButton(const String& label, const String& target)
{
  webServer.sendContent(
    "<div style='text-align:center; margin:25px 0;'>"
    "  <a href='" + target + "' class='mobile-btn'>" +
    label +
    "</a>"
    "</div>"
  );
}

void WwwServer::sendTitle(const String& text)
{
  webServer.sendContent(
    "<div class='page-title'>"
    "  <h1>" + text + "</h1>"
    "</div>"
  );
}

void WwwServer::sendDrawPage()
{
  webServer.sendContent(
  "<div style='text-align:center;'>"

  // Color Picker
  "Color<br>"
  "<input type='color' id='colorPicker' value='#ff0000'><br><br>\n"

  // Canvas
  "<canvas id='c' width='64' height='32' "
  "style='width:320px;height:160px;"
  "border:2px solid #4CAF50;"
  "background:black;"
  "image-rendering:pixelated;"
  "touch-action:none;'>"
  "</canvas><br><br>\n"

  // First row: Clear & Send buttons
  "<div style='display:flex;justify-content:center;gap:10px;'>"
  "<button onclick='clearCanvas()'>Clear</button>"
  "<button onclick='sendFrame()'>Send</button>"
  "</div><br>\n"

  // Second row: Brightness slider + Dithering checkbox
  "<div style='display:flex;justify-content:center;align-items:center;gap:10px;'>"
  "Brightness <input type='range' id='brightness' min='0' max='100' value='100'>"
  "<label><input type='checkbox' id='ditherToggle' checked> Dithering</label>"
  "</div><br>\n"

  // Presets
  "<b>Presets</b><br>"
  "<button onclick='presetNoise()'>Noise</button>"
  "<button onclick='presetGradient()'>Gradient</button>"
  "<button onclick='presetPlasma()'>Plasma</button>"
  "<button onclick='presetMandelbrot()'>Mandelbrot</button>"

  "</div>\n"

  // --- SCRIPT ---
  "<script>\n"

  // 4x4 Bayer matrix for dithering
  "const bayer4x4=[[0,8,2,10],[12,4,14,6],[3,11,1,9],[15,7,13,5]];"

  "const canvas=document.getElementById('c');"
  "const ctx=canvas.getContext('2d');"
  "ctx.fillStyle='black';ctx.fillRect(0,0,64,32);"

  "let drawing=false;\n"
  "let canvasDirty=false;\n"

  "function getPos(e){"
  " const r=canvas.getBoundingClientRect();"
  " const x=Math.floor((e.touches?e.touches[0].clientX:e.clientX)-r.left);"
  " const y=Math.floor((e.touches?e.touches[0].clientY:e.clientY)-r.top);"
  " return {x:Math.floor(x*64/r.width), y:Math.floor(y*32/r.height)};"
  "}\n"

  "function drawPixel(e){"
  " const p=getPos(e);"
  " ctx.fillStyle=document.getElementById('colorPicker').value;"
  " ctx.fillRect(p.x,p.y,1,1);"
  " canvasDirty=true;"
  "}\n"

  // Canvas event handlers
  "canvas.onmousedown=(e)=>{drawing=true;drawPixel(e);};"
  "canvas.onmouseup=()=>drawing=false;"
  "canvas.onmousemove=(e)=>{if(drawing)drawPixel(e);};"
  "canvas.ontouchstart=(e)=>{drawing=true;drawPixel(e);};"
  "canvas.ontouchend=()=>drawing=false;"
  "canvas.ontouchmove=(e)=>{if(drawing){drawPixel(e);e.preventDefault();}};\n"

  // Clear canvas
  "function clearCanvas(){ctx.fillStyle='black';ctx.fillRect(0,0,64,32); canvasDirty=true;}"

  // Presets
  "function presetNoise(){"
  " const img=ctx.getImageData(0,0,64,32);"
  " for(let i=0;i<img.data.length;i+=4){"
  "   img.data[i]=Math.random()*255;"
  "   img.data[i+1]=Math.random()*255;"
  "   img.data[i+2]=Math.random()*255;"
  " } ctx.putImageData(img,0,0); canvasDirty=true;"
  "}\n"

  // Render the Mandelbrot set
  "function presetMandelbrot() {\n"
  "  const img = ctx.createImageData(64, 32);  // ImageData to manipulate pixels \n"
  "  const maxIter = 100;  // Maximum number of iterations \n"
  "  const zoom = 10;     // Zoom level \n"
  "  const moveX = -0.5;   // Shift X axis \n"
  "  const moveY = 0;      // Shift Y axis \n"
  " \n"
  "  for (let y = 0; y < 32; y++) { \n"
  "    for (let x = 0; x < 64; x++) { \n"
  "      const cx = (x - 32) / zoom + moveX;  // Scale and shift the X-coordinate \n"
  "      const cy = (y - 16) / zoom + moveY;  // Scale and shift the Y-coordinate \n"
  " \n"
  "      let zx = 0.0; \n"
  "      let zy = 0.0; \n"
  "      let iteration = 0; \n"
  " \n"
  "      // Mandelbrot calculation \n"
  "      while (zx * zx + zy * zy < 4 && iteration < maxIter) { \n"
  "        const tempX = zx * zx - zy * zy + cx; \n"
  "        zy = 2.0 * zx * zy + cy; \n"
  "        zx = tempX; \n"
  "        iteration++; \n"
  "      } \n"
  " \n"
  "      // Color based on iteration count (how fast it diverged) \n"
  "      const pixelIndex = (y * 64 + x) * 4; \n"
  "      let color = 100;  // Default to black (inside set) \n"
  " \n"
  "      if (iteration < maxIter) { \n"
  "        color = Math.floor((iteration / maxIter) * 255); \n"
  "      } \n"
  " \n"
  "      img.data[pixelIndex] = color;        // Red \n"
  "      img.data[pixelIndex + 1] = color;    // Green \n"
  "      img.data[pixelIndex + 2] = color;    // Blue \n"
  "      img.data[pixelIndex + 3] = 255;      // Alpha (fully opaque) \n"
  "    } \n"
  "  }"
  " \n"
  "  // Put the fractal data on the canvas \n"
  "  ctx.putImageData(img, 0, 0); \n"
  "  canvasDirty = true;  // Mark canvas as dirty \n"
  "} \n"

  "function presetGradient(){"
  " const img=ctx.createImageData(64,32);"
  " for(let y=0;y<32;y++){"
  "   for(let x=0;x<64;x++){"
  "     const i=(y*64+x)*4;"
  "     img.data[i]=x*4; img.data[i+1]=y*8; img.data[i+2]=128; img.data[i+3]=255;"
  "   }}"
  " ctx.putImageData(img,0,0); canvasDirty=true;"
  "}\n"

  "function presetPlasma(){"
  " const img=ctx.createImageData(64,32);"
  " for(let y=0;y<32;y++){"
  "   for(let x=0;x<64;x++){"
  "     const i=(y*64+x)*4;"
  "     const v=Math.sin(x/4)+Math.sin(y/4);"
  "     img.data[i]=(v+2)*60; img.data[i+1]=(v+2)*40; img.data[i+2]=(v+2)*30; img.data[i+3]=255;"
  "   }}"
  " ctx.putImageData(img,0,0); canvasDirty=true;"
  "}\n"

  // Send frame with optional dithering
  "function sendFrame(){"
  " const img=ctx.getImageData(0,0,64,32).data;"
  " const brightness=document.getElementById('brightness').value/100;"
  " const dither=document.getElementById('ditherToggle').checked;"
  " let bytes=[];"
  " for(let y=0;y<32;y++){"
  "   for(let x=0;x<64;x++){"
  "     const i=(y*64+x)*4;"
  "     let r=img[i]*brightness;"
  "     let g=img[i+1]*brightness;"
  "     let b=img[i+2]*brightness;"
  "     if(dither){"
  "       const t=bayer4x4[y%4][x%4]/16;"
  "       r=Math.min(31,Math.max(0,((r*31/255 + t)|0)));"
  "       g=Math.min(63,Math.max(0,((g*63/255 + t)|0)));"
  "       b=Math.min(31,Math.max(0,((b*31/255 + t)|0)));"
  "     } else {"
  "       r=r*31/255|0; g=g*63/255|0; b=b*31/255|0;"
  "     }"
  "     const rgb565=(r<<11)|(g<<5)|b;"
  "     bytes.push(rgb565&0xFF);"
  "     bytes.push(rgb565>>8);"
  "   }}"
  " let binary=String.fromCharCode(...bytes);"
  " let b64=btoa(binary);"
  " fetch('/frame',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'data='+encodeURIComponent(b64)});"
  "}\n"

  // Auto-send while drawing, throttled ~15 FPS
  "setInterval(()=>{ if(canvasDirty){canvasDirty=false; sendFrame(); } },66); \n"

  "</script>\n"
  );
}

void WwwServer::handleClient()
{
    webServer.handleClient();
}

RequestHandler& WwwServer::on(const Uri &uri, WebServer::THandlerFunction fn)
{
    return webServer.on(uri, fn);
}

RequestHandler& WwwServer::on(const Uri &uri, HTTPMethod method, WebServer::THandlerFunction fn)
{
    return webServer.on(uri, method, fn);
}

RequestHandler& WwwServer::on(const Uri &uri, HTTPMethod method, WebServer::THandlerFunction fn, WebServer::THandlerFunction ufn)
{
    return webServer.on(uri, method, fn, ufn);
}

void WwwServer::sendContent(const String &content)
{
    webServer.sendContent(content);
}

void WwwServer::sendContent(const char *content, size_t contentLength)
{
    webServer.sendContent(content, contentLength);
}

void WwwServer::sendContent_P(PGM_P content)
{
    webServer.sendContent(content);
}

void WwwServer::sendContent_P(PGM_P content, size_t size)
{
    webServer.sendContent(content, size);
}

String WwwServer::uri() const
{
    return webServer.uri();
}
  
HTTPMethod WwwServer::method() const
{
    return webServer.method();
}

NetworkClient &WwwServer::client()
{
    return webServer.client();
}
  
HTTPUpload &WwwServer::upload()
{
    return webServer.upload();
}

HTTPRaw &WwwServer::raw()
{
    return webServer.raw();
}

void WwwServer::send(int code, const char *content_type, const String &content)
{
  webServer.send(code, content_type, content);
}

void WwwServer::send(int code, char *content_type, const String &content)
{
  webServer.send(code, content_type, content);
}

void WwwServer::send(int code, const String &content_type, const String &content)
{
  webServer.send(code, content_type, content);
}

void WwwServer::send(int code, const char *content_type, const char *content)
{
  webServer.send(code, content_type, content);
}

String WwwServer::arg(const String &name) const
{
  return webServer.arg(name);  
}

String WwwServer::arg(int i) const
{
  return webServer.arg(i);
}

int WwwServer::args() const
{
  return webServer.args();
}

String WwwServer::argName(int i) const
{
  return webServer.argName(i);
}

bool WwwServer::hasArg(const String &name) const
{
  return webServer.hasArg(name);
}

String WwwServer::htmlEscape(const String& s)
{
    String out = s;
    out.replace("&", "&amp;");
    out.replace("<", "&lt;");
    out.replace(">", "&gt;");
    out.replace("\"", "&quot;");
    return out;
}
