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
    "<meta http-equiv='refresh' content='10'/>"
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
