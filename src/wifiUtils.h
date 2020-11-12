
#ifndef WIFIUTILS_H
#define WIFIUTILS_H
#include <Arduino.h>
#include <vector>
#include <string>
#include <ArduinoJson.h>
#if defined ESP32
#include <WiFi.h> // Include the Wi-Fi library
#endif

#if defined ESP8266
#include <ESP8266WiFi.h>
#endif
#include <DNSServer.h> //Local DNS Server used for redirecting all requests to the configuration portal
#if defined ESP32
#include "SPIFFS.h"
#endif
#if defined ESP8266
#include <FS.h>
#endif

#define WIFI_FILE_NAME "/wifi.json"
#if !defined(WIFI_MODE_STA)
#define WIFI_MODE_STA WIFI_STA
#endif

class WifiUtils
{

    byte DNS_PORT;

    DNSServer dnsServer;

    DynamicJsonDocument _WIFI_CONFIG_JSON;
    // extract the AP names from listPlus vector that has ap, rssi, isOpen - Paul
    std::vector<String> extractAPnames(std::vector<String> listPlus);

    std::vector<String> getLocalWifiAps();

    int connectToWifi(char *apName, char *pass);

    bool tryWifis();

    void writeToApFile(String out);

    String getEmptyConfigString();

    void initWifiFile();

    File getConfigFile();

    void initializeGlobalJsonConfig(String strJson);

    void readConfigFileIntoGlobalJson(File file);

    JsonArray getApsStored();

    String _ipAddr;
    String _apName;
    String _selfApName;
    bool _dsnRunning;

    void startupWifiConfigSites();
    void writeJsonConfigToFile(DynamicJsonDocument doc);

public:
    WifiUtils() : _WIFI_CONFIG_JSON(4096)
    {
        this->_ipAddr = "";
        this->_apName = "";
        this->DNS_PORT = 53;
        this->_selfApName = "";
    };
    void initialize();
    String getIp()
    {
        return this->_ipAddr;
    }

    String getApName()
    {
        return this->_apName;
    }

    String getMac()
    {
        return WiFi.macAddress();
    }

    String getWiFiScan(bool async);
    void updateAp(JsonObject &jsonObj);
    void forgetAp(String jsonIn);
    void processNextDNSRequest();
    void showJson();
    void disconnectWiFi();
    void justStation()
    {
        WiFi.mode(WIFI_MODE_STA);
    }
    void setSelfAPName(String s)
    {
        this->_selfApName = String(s);
    }
    std::vector<String> getlistPlus(int n);
    std::vector<String> scanLocalWifi()
    {
        //WiFi.scanDelete();
        //String json = "[";
        std::vector<String> listPlus;
        int n = WiFi.scanComplete();
        if (n == -2)
        {
            WiFi.scanNetworks(true);
        }
        else if (n)
        {
            listPlus = getlistPlus(n);
            /* for (int i = 0; i < n; ++i)
            {
                if (i)
                    json += ",";
                json += "{";
                json += "\"rssi\":" + String(WiFi.RSSI(i));
                json += ",\"ssid\":\"" + WiFi.SSID(i) + "\"";
                json += ",\"bssid\":\"" + WiFi.BSSIDstr(i) + "\"";
                json += ",\"channel\":" + String(WiFi.channel(i));
                json += ",\"secure\":" + String(WiFi.encryptionType(i));
                json += ",\"hidden\":" + String(WiFi.isHidden(i) ? "true" : "false");
                json += "}";
            } */
            WiFi.scanDelete();
            if (WiFi.scanComplete() == -2)
            {
                WiFi.scanNetworks(true);
            }
        }
        //json += "]";
        //request->send(200, "application/json", json);
        //json = String();
        //return json;
        return listPlus;
    }
};
#endif
