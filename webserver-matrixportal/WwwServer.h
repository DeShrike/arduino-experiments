#ifndef _WWWSERVER_H_
#define _WWWSERVER_H_

#include <WiFi.h>
#include <NetworkClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

class WwwServer
{
  private:
    WebServer webServer;
    bool started = false;

    void decode(const char *encoded, char *result, size_t result_size);
    void handleNotFound();
    String htmlEscape(const String& s);

  public:
    unsigned long requestCount = 0;

    WwwServer();
    WwwServer(const WwwServer&) = delete;
    WwwServer& operator=(const WwwServer&) = delete;

    RequestHandler &on(const Uri &uri, WebServer::THandlerFunction fn);
    RequestHandler &on(const Uri &uri, HTTPMethod method, WebServer::THandlerFunction fn);
    RequestHandler &on(const Uri &uri, HTTPMethod method, WebServer::THandlerFunction fn, WebServer::THandlerFunction ufn);  //ufn handles file uploads

    String localIP();
    size_t contentLength();
    
    bool init(const char *ssid, const char *pw);
    void begin();
    void handleClient();

    void sendContent(const String &content);
    void sendContent(const char *content, size_t contentLength);
    void sendContent_P(PGM_P content);
    void sendContent_P(PGM_P content, size_t size);

    void send(int code, const char *content_type = NULL, const String &content = String(""));
    void send(int code, char *content_type, const String &content);
    void send(int code, const String &content_type, const String &content);
    void send(int code, const char *content_type, const char *content);

    void sendHtmlHeader();
    void sendHtmlFooter();
    void sendHtmlForm(const String& currentText, const String& currentColor);
    void sendFormPostScript();
    void sendFormStyle();
    void sendStatusPage(const String& currentText, const String& currentColor);
    void sendNavButton(const String& label, const String& target);
    void sendTitle(const String& text);
    void sendDrawPage();


    String uri() const;
    HTTPMethod method() const;
    NetworkClient &client();
    HTTPUpload &upload();
    HTTPRaw &raw();

    String arg(const String &name) const;
    String arg(int i) const;
    int args() const;
    String argName(int i) const;
    bool hasArg(const String &name) const;

};

#endif
