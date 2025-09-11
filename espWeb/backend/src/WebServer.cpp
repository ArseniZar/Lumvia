#include "WebServer.h"

namespace espweb
{
    WebServer::WebServer(Logger &logger, int port) : logger(logger), server(port)
    {
        MDNS.addService(F("http"), F("tcp"), port);
    }

    /*========================================begin=============================================================*/

    void WebServer::begin(std::function<std::vector<api::Network>()> scanAvailableWiFiNetworks,
                          std::function<bool(const char *, const char *)> attemptConnection)
    {
        server.keepAlive(true);

        server.on(F("/"), HTTP_GET, [this]()
                  { this->handleRoot(); });

        server.on(F("/"), HTTP_OPTIONS, [this]()
                  {    
                       server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                       server.sendHeader(F("Access-Control-Allow-Methods"), F("GET, OPTIONS"));
                       server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
                       server.send(200); });

        server.on(F("/scan"), HTTP_GET, [this, scanAvailableWiFiNetworks]()
                  { this->handleScan(scanAvailableWiFiNetworks); });

        server.on(F("/scan"), HTTP_OPTIONS, [this]()
                  {
                       server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                       server.sendHeader(F("Access-Control-Allow-Methods"), F("GET, OPTIONS"));
                       server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
                       server.send(200); });

        server.on(F("/network"), HTTP_POST, [this, attemptConnection]()
                  { this->handleNetwork(attemptConnection); });

        server.on(F("/network"), HTTP_OPTIONS, [this]()
                  {
                       server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                       server.sendHeader(F("Access-Control-Allow-Methods"), F("POST, OPTIONS"));
                       server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
                       server.send(200); });

        server.on(F("/end"), HTTP_GET, [this]()
                  { this->handleEnd(); });

        server.on(F("/end"), HTTP_OPTIONS, [this]()
                  {
                       server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                       server.sendHeader(F("Access-Control-Allow-Methods"), F("GET, OPTIONS"));
                       server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
                       server.send(200); });

        server.onNotFound([this]()
                          { this->handleNotFound(); });

        server.begin();
        serverRunning = true;
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf.add(F("(WebServer::begin) Server started and running.")); return buf; });
    }

    /*========================================handleRoot=============================================================*/

    void WebServer::handleRoot()
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleRoot) Handling root page request.")); return buf; });
        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        server.send_P(200, "text/html", index);
    }

    /*========================================handleScan=============================================================*/

    void WebServer::handleScan(std::function<std::vector<api::Network>()> scanAvailableWiFiNetworks)
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleScan) Processing scan networks request.")); return buf; });
        const auto responce = api::ApiSuccessResponse<api::ScanNetworksResponce>(200, std::move(api::ScanNetworksResponce(std::move(scanAvailableWiFiNetworks()), std::move(api::ModelBaseResponse()))));
        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        server.send(responce.getCode(), F("application/json"), api::serializeApiResponse(responce));
    }

    /*========================================handleNetwork==========================================================*/

    void WebServer::handleNetwork(std::function<bool(const char *, const char *)> attemptConnection)
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleNetwork) Received network connect request.")); return buf; });

        const String &body = server.arg(F("plain"));

        if (body.isEmpty())
        {
            logger.log(LOG_WARN, [&]() -> String128
                       {String128 buf; buf.add(F("(WebServer::handleNetwork) Empty request body.")); return buf; });
            const auto responce = api::ApiErrorResponse(400, String32(F("Empty request body")));
            server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
            server.send(responce.getCode(), F("application/json"), api::serializeApiResponse(responce));
            return;
        }

        auto apiRequstPtr = api::parseApiRequest<api::ConnectNetworkRequest>(body.c_str());
        if (apiRequstPtr->isOk())
        {
            auto *successApiRequstPtr = static_cast<api::ApiSuccessRequest<api::ConnectNetworkRequest> *>(apiRequstPtr.get());
            const bool status = attemptConnection(successApiRequstPtr->data.network.ssid, successApiRequstPtr->data.network.password);
            const auto responce = api::ApiSuccessResponse<api::ConnectNetworkResponce>(200, std::move(api::ConnectNetworkResponce(status, std::move(successApiRequstPtr->data))));
            server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
            server.send(responce.getCode(), F("application/json"), api::serializeApiResponse(responce));
        }
        else
        {
            auto *errorApiRequstPtr = static_cast<api::ApiErrorRequest *>(apiRequstPtr.get());
            const auto responce = api::ApiErrorResponse(400, errorApiRequstPtr->message);
            server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
            server.send(responce.getCode(), F("application/json"), api::serializeApiResponse(responce));
        }
    }

    /*========================================handleEnd============================================================*/

    void WebServer::handleEnd()
    {
        logger.log(LOG_INFO, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleEnd) Request accepted, stopping server.")); return buf; });
        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        server.send(200);
        stop();
    }

    /*========================================handleNotFound========================================================*/

    void WebServer::handleNotFound()
    {
        logger.log(LOG_WARN, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleNotFound) Resource not found.")); return buf; });
        const auto responce = api::ApiErrorResponse(400, String32(F("Resource not found")));
        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        server.send(responce.getCode(), F("application/json"), api::serializeApiResponse(responce));
    }

    /*========================================handleClient==========================================================*/

    void WebServer::handleClient()
    {
        server.handleClient();
    }

    /*==============================================stop=============================================================*/

    void WebServer::stop()
    {
        server.stop();
        serverRunning = false;
        logger.log(LOG_INFO, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::stop) Stop server.")); return buf; });
    }

    /*========================================isRunning=============================================================*/

    bool WebServer::isRunning() const
    {
        return serverRunning;
    }
}
