#pragma once
#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Arduino.h>
#include <StringN.h>
#include <vector>
#include "Logger.h"
#include "WebServer.h"

#ifndef WIFI_CONNECTION_TIMEOUT_MS
#define WIFI_CONNECTION_TIMEOUT_MS 10000
#endif

enum class ConnState : int
{
    WL_NO_SHIELD = 255,
    WL_IDLE_STATUS = 0,
    WL_NO_SSID_AVAIL = 1,
    WL_SCAN_COMPLETED = 2,
    WL_CONNECTED = 3,
    WL_CONNECT_FAILED = 4,
    WL_CONNECTION_LOST = 5,
    WL_WRONG_PASSWORD = 6,
    WL_DISCONNECTED = 7
};

class WiFiSetup
{
public:
    bool begin();
    static WiFiSetup &init(Logger &logger);
    bool attemptConnection(const char *ssid, const char *password);
    void setAPConfig(const char *apSsid, const char *apPassword);
    void setWiFiConfig(const char *ssid, const char *password);
    void setMdnsName(const char *mdnsName);
    StringN<18> getMAC() const;
    const char *getSsid() const;
    const char *getPass() const;
    ConnState status();

private:
    WiFiSetup(Logger &logger);

    Logger &logger;
    espweb::WebServer webserver;

    String32 apSsid;
    String32 apPassword;
    String32 ssid;
    String32 password;
    String32 mdnsName;

    bool stopAP();
    bool stopMDNS();
    void initServer();
    std::vector<api::Network> scanAvailableWiFiNetworks();
    bool startMDNS(const String32 &mdnsName);
    bool tryConnectWifi(const String32 &ssid, const String32 &password);
    bool startAP(const String32 &apSsid, const String32 &apPassword);
};

#endif // WIFI_SETUP_H
