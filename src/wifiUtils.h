
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

    void startupWifiConfigSites();
    void writeJsonConfigToFile(DynamicJsonDocument doc);

public:
    WifiUtils() : _WIFI_CONFIG_JSON(4096)
    {
        this->_ipAddr = "";
        this->_apName = "";
        this->DNS_PORT = 53;
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

    String getWiFiScan();
    void updateAp(String jsonIn);
    void forgetAp(String jsonIn);
    void processNextDNSRequest();
    void showJson();
    void disconnectWiFi();
    void justStation()
    {
        WiFi.mode(WIFI_MODE_STA);
    }
};
#endif
