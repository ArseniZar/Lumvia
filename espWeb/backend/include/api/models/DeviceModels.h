#pragma once
#ifndef DEVICE_MODELS_H
#define DEVICE_MODELS_H

#include <Arduino.h>
#include "JsonModelBase.h"

namespace api
{ /*===================================== Network ===========================================================*/

    class Network
    {
    public:
        String32 ssid;
        String32 password;
        int rssi;
        int encryptionType;
        int channel;
        StringN<18> bssid;
        bool hidden;

        Network() = delete;
        Network(const Network &other) = default;
        Network(Network &&other) = default;
        Network(const char *ssid, const char *password, int rssi, int encryptionType, int channel, const char *bssid, bool hidden)
            : ssid(ssid),
              password(password),
              rssi(rssi),
              encryptionType(encryptionType),
              channel(channel),
              bssid(bssid),
              hidden(hidden)
        {
        }
    };

    /*===================================== ScanNetwork =======================================================*/

    // class ScanNetworkRequest final : public ModelBaseRequest
    // {
    // public:
    //     ScanNetworkRequest() = delete;
    //     ScanNetworkRequest(ModelBaseRequest &&base);
    //     ScanNetworkRequest(const ModelBaseRequest &base);
    //     static std::unique_ptr<JsonConvertible> fromJson(const gson::Entry &data);
    // };

    class ScanNetworksResponce final : public ModelBaseResponse
    {
    public:
        std::vector<Network> networks;
        ScanNetworksResponce() = delete;
        ScanNetworksResponce(const std::vector<Network> &networks, const ModelBaseResponse &base);
        ScanNetworksResponce(std::vector<Network> &&networks, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };

    /*===================================== ConnectNetwork ====================================================*/

    class ConnectNetworkRequest final : public ModelBaseRequest
    {
    public:
        Network network;
        ConnectNetworkRequest() = delete;
        ConnectNetworkRequest(Network &&network, ModelBaseRequest &&base);
        ConnectNetworkRequest(const Network &network, const ModelBaseRequest &base);
        static std::unique_ptr<JsonConvertible> fromJson(const gson::Entry &data);
    };

    class ConnectNetworkResponce final : public ModelBaseResponse
    {
    public:
        bool status;
        ConnectNetworkResponce() = delete;
        ConnectNetworkResponce(const bool status, const ModelBaseResponse &base);
        ConnectNetworkResponce(bool status, ModelBaseResponse &&base);
        gson::Str toJson() const override;
    };
}
#endif // DEVICE_MODELS_H