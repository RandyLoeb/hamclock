#ifndef WIFICONFIGWEBSERVER_H
#define WIFICONFIGWEBSERVER_H
#include "ESPAsyncWebServer.h"
#include "wifiUtils.h"
#if !defined FILE_READ
#define FILE_READ "r"
#endif
class KeyerWebServer
{

    AsyncWebServer server;

    /* void initializeServer();

    String getPage(String page);
    String getWifiAdminPage();
    String getWifiAdminCSS();

    String getWifiAdminJS(); */

public:
    WifiUtils *_wifiUtils;

    void start()
    {
        this->initializeServer();
        this->server.begin();
    }

    void showUtilJson()
    {
        this->_wifiUtils->showJson();
    }

    void initializeServer()
    {
        //server.enableCORS(true);
        //server.enableCrossOrigin(true);
        //WifiUtils * myutils = this->_wifiUtils;
        server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) { request->send(SPIFFS, "/wifiadmin.html", "text/html"); }); // Call the 'handleRoot' function when a client requests URI "/"
        server.on("/wifiadmin.html", HTTP_GET, [this](AsyncWebServerRequest *request) { request->send(SPIFFS, "/wifiadmin.html", "text/html"); });
        server.on("/wifiadmin.css", HTTP_GET, [this](AsyncWebServerRequest *request) { request->send(SPIFFS, "/wifiadmin.css", "text/css"); });
        server.on("/wifiadmin.js", HTTP_GET, [this](AsyncWebServerRequest *request) { request->send(SPIFFS, "/wifiadmin.js", "text/javascript"); });

        server.on("/wifiscan", [this](AsyncWebServerRequest *request) {
        String scanJson = this->_wifiUtils->getWiFiScan();
        request->send(200, "application/json", scanJson); });

        //nothing in first lambda blocks, and dont remove it
        server.on(
            "/updateap", HTTP_POST, [this](AsyncWebServerRequest *request) {}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
     String s = String((const char*)data);
  
      this->_wifiUtils->updateAp(s);
        request->send(200, "text/html", "ok"); });

        server.on(
            "/forgetap", HTTP_POST, [this](AsyncWebServerRequest *request) {}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
     String s = String((const char*)data);
  
      this->_wifiUtils->forgetAp(s);
        request->send(200, "text/html", "ok"); });
        server.on("/reboot", [this](AsyncWebServerRequest *request) {
         request->send(200, "text/html", "rebooting");
        ESP.restart(); });
        server.on("/keyer.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
            //this->preSPIFFS();
            //request->onDisconnect([this]() { this->postSPIFFS(); });

            //request->send(200, "text/html", "ok");
            request->send(SPIFFS, "/keyer.html", "text/html");
            //this->postSPIFFS();
        });

        server.on("/jquery-3.5.1.min.js", HTTP_GET, [this](AsyncWebServerRequest *request) {
            // this->preSPIFFS();
            //request->onDisconnect([this]() { this->postSPIFFS(); });
            request->send(SPIFFS, "/jquery-3.5.1.min.js", "text/javascript");
            //this->postSPIFFS();
        });
        server.on("/knockout-3.5.1.min.js", HTTP_GET, [this](AsyncWebServerRequest *request) {
            //this->preSPIFFS();
            //request->onDisconnect([this]() { this->postSPIFFS(); });
            request->send(SPIFFS, "/knockout-3.5.1.min.js", "text/javascript");
            //this->postSPIFFS();
        });
        server.on("/axios.min.js", HTTP_GET, [this](AsyncWebServerRequest *request) {
            //this->preSPIFFS();
            //request->onDisconnect([this]() { this->postSPIFFS(); });
            request->send(SPIFFS, "/axios.min.js", "text/javascript");
            //this->postSPIFFS();
        });

        server.onNotFound([this](AsyncWebServerRequest *request) { request->redirect("http://" + WiFi.softAPIP().toString() + "/"); });
    }

    String getPage(String page)
    {
        String pageBody = "";
        File f = SPIFFS.open(page, FILE_READ);
        if (!f)
        {
            Serial.println("problem opening " + page);
        }
        else
        {

            while (f.available())
            {
                pageBody = pageBody + f.readString();
            }
            f.close();
            //Serial.println(pageBody);
        }
        return pageBody;
    }
    String getWifiAdminPage()
    {
        return getPage("/wifiadmin.html");
    }

    String getWifiAdminCSS()
    {
        return getPage("/wifiadmin.css");
    }

    String getWifiAdminJS()
    {
        return getPage("/wifiadmin.js");
    }
};

#endif