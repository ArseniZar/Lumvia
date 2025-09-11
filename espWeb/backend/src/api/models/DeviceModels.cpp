#include "DeviceModels.h"

namespace api
{
    /*===================================== ScanNetwork ==========================================================*/

    // namespace ScanNetworkRequestKey
    // {

    // }

    // ScanNetworkRequest::ScanNetworkRequest(const ModelBaseRequest &base) : ModelBaseRequest(base) {}
    // ScanNetworkRequest::ScanNetworkRequest(ModelBaseRequest &&base) : ModelBaseRequest(std::move(base)) {}

    // std::unique_ptr<JsonConvertible> ScanNetworkRequest::fromJson(const gson::Entry &data)
    // {
    //     using namespace ScanNetworkRequestKey;
    //     static const std::map<const char *, gson::Type> keys = {};

    //     auto parseBasePtr = ModelBaseRequest::fromJson(data);
    //     if (!parseBasePtr->isOk())
    //     {
    //         return parseBasePtr;
    //     }
    //     auto *successParseBasePtr = static_cast<JsonParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());

    //     auto parseScanNetworkPtr = JsonParse::parseData(data, keys);
    //     if (!parseScanNetworkPtr->isOk())
    //     {
    //         return parseScanNetworkPtr;
    //     }
    //     auto *successParseScanNetworkPtr = static_cast<JsonParseSuccess<std::map<const char *, String>> *>(parseScanNetworkPtr.get());
    //     std::map<const char *, String> _ = std::move(successParseScanNetworkPtr->result);
    //     return std::make_unique<JsonParseSuccess<ScanNetworkRequest>>(std::move(ScanNetworkRequest(std::move(successParseBasePtr->result))));
    // }

    /*--------------------------------------------------------------------------------------------------------------*/

    namespace ScanNetworksResponceKey
    {
        constexpr const char *SSID = "ssid";
        constexpr const char *PASSWORD = "password";
        constexpr const char *RSSI = "rssi";
        constexpr const char *ENCRYPTION = "encryptionType";
        constexpr const char *CHANNEL = "channel";
        constexpr const char *BSSID = "bssid";
        constexpr const char *HIDDEN = "hidden";
        constexpr const char *NETWORKS = "networks";
    }

    ScanNetworksResponce::ScanNetworksResponce(const std::vector<Network> &networks, const ModelBaseResponse &base) : ModelBaseResponse(base), networks(networks) {};
    ScanNetworksResponce::ScanNetworksResponce(std::vector<Network> &&networks, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), networks(std::move(networks)) {};

    gson::Str ScanNetworksResponce::toJson() const
    {
        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[ScanNetworksResponceKey::NETWORKS]('[');

        for (size_t i = 0; i < networks.size(); i++)
        {
            const int size = 7;
            using namespace ScanNetworksResponceKey;
            std::tuple<const char *, gson::Type, String> pairs[size] = {
                {SSID, gson::Type::String, networks[i].ssid.c_str()},
                {PASSWORD, gson::Type::String, networks[i].password.c_str()},
                {RSSI, gson::Type::Int, String(networks[i].rssi)},
                {ENCRYPTION, gson::Type::Int, String(networks[i].encryptionType)},
                {CHANNEL, gson::Type::Int, String(networks[i].channel)},
                {BSSID, gson::Type::String, networks[i].bssid.c_str()},
                {HIDDEN, gson::Type::Bool, networks[i].hidden ? "true" : "false"}};

            j += JsonSerialization::serializationJson(pairs, size);
        }
        j(']');
        return j;
    }

    /*===================================== ConnectNetwork ==========================================================*/

    namespace ConnectNetworkRequestKey
    {
        constexpr const char *SSID = "ssid";
        constexpr const char *PASSWORD = "password";
        constexpr const char *RSSI = "rssi";
        constexpr const char *ENCRYPTION = "encryptionType";
        constexpr const char *CHANNEL = "channel";
        constexpr const char *BSSID = "bssid";
        constexpr const char *HIDDEN = "hidden";
    }

    ConnectNetworkRequest::ConnectNetworkRequest(const Network &network, const ModelBaseRequest &base) : ModelBaseRequest(base), network(network) {}
    ConnectNetworkRequest::ConnectNetworkRequest(Network &&network, ModelBaseRequest &&base) : ModelBaseRequest(std::move(base)), network(std::move(network)) {}

    std::unique_ptr<JsonConvertible> ConnectNetworkRequest::fromJson(const gson::Entry &data)
    {
        using namespace ConnectNetworkRequestKey;
        static const std::map<const char *, gson::Type> keys = {
            {SSID, gson::Type::String},
            {PASSWORD, gson::Type::String},
            {RSSI, gson::Type::Int},
            {ENCRYPTION, gson::Type::Int},
            {CHANNEL, gson::Type::Int},
            {BSSID, gson::Type::String},
            {HIDDEN, gson::Type::Bool}};

        auto parseBasePtr = ModelBaseRequest::fromJson(data);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        auto *successParseBasePtr = static_cast<JsonParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());

        auto parseConnectNetworkPtr = JsonParse::parseData(data, keys);
        if (!parseConnectNetworkPtr->isOk())
        {
            return parseConnectNetworkPtr;
        }
        auto *successParseConnectNetworkPtr = static_cast<JsonParseSuccess<std::map<const char *, String>> *>(parseConnectNetworkPtr.get());
        std::map<const char *, String> map = std::move(successParseConnectNetworkPtr->result);
        return std::make_unique<JsonParseSuccess<ConnectNetworkRequest>>(std::move(ConnectNetworkRequest(std::move(Network(map[SSID].c_str(), map[PASSWORD].c_str(), map[RSSI].toInt(), map[ENCRYPTION].toInt(), map[CHANNEL].toInt(), map[BSSID].c_str(), map[HIDDEN].equalsIgnoreCase("true"))), std::move(successParseBasePtr->result))));
    }

    /*--------------------------------------------------------------------------------------------------------------*/

    namespace ConnectNetworkResponceKey
    {
        constexpr const char *STATUS = "status";
    }

    ConnectNetworkResponce::ConnectNetworkResponce(const bool status, const ModelBaseResponse &base) : ModelBaseResponse(base), status(status) {}
    ConnectNetworkResponce::ConnectNetworkResponce(bool status, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), status(status) {}

    gson::Str ConnectNetworkResponce::toJson() const
    {
        const int size = 1;
        using namespace ConnectNetworkResponceKey;
        std::tuple<const char *, gson::Type, String> pairs[size] = {
            {STATUS, gson::Type::Bool, status ? "true" : "false"}};

        gson::Str j;
        j += ModelBaseResponse::toJson();
        j[STATUS] = JsonSerialization::serializationJson(pairs, size);
        return j;
    }
}
