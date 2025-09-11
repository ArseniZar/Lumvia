#pragma once
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <StringN.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <functional>
#include "Logger.h"
#include "index.h"
#include "DeviceModels.h"
#include "ApiParse.h"
#include "ApiSerialization.h"

namespace espweb
{
    class WebServer
    {
    public:
        WebServer() = delete;
        WebServer(Logger &logger, int port);

        void stop();
        void handleClient();
        bool isRunning() const;
        void begin(std::function<std::vector<api::Network>()> scanAvailableWiFiNetworks,
                   std::function<bool(const char *, const char *)> attemptConnection);

    private:
        Logger &logger;
        ESP8266WebServer server;

        bool serverRunning = false;
        
        void handleEnd();
        void handleRoot();
        void handleNotFound();
        void handleScan(std::function<std::vector<api::Network>()> scanAvailableWiFiNetworks);
        void handleNetwork(std::function<bool(const char *, const char *)> attemptConnection);
    };

}
#endif // WEBSERVER_H
