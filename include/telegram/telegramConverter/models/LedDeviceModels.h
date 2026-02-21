#pragma once
#ifndef LED_DEVICE_MODELS_H
#define LED_DEVICE_MODELS_H

#include <Arduino.h>
#include "MessageModelsBase.h"

namespace telegram
{
    /*===================================== UpdateLedDevice =======================================================*/

    class UpdateLedDeviceRequest final : public ModelBaseRequest
    {
    public:
        StringN<8> color;
        bool status;
        UpdateLedDeviceRequest() = delete;
        UpdateLedDeviceRequest(const char *color, const bool status, const ModelBaseRequest &base);
        UpdateLedDeviceRequest(const char *color, const bool status, ModelBaseRequest &&base);
        static std::unique_ptr<MessageConvertible> fromMessage(const su::Text *pairs, const int pairsSize);
    };

    /*===================================== ScanLedDevice =========================================================*/

    class ScanLedDeviceRequest final : public ModelBaseRequest
    {
    public:
        ScanLedDeviceRequest() = delete;
        ScanLedDeviceRequest(ModelBaseRequest &&base);
        ScanLedDeviceRequest(const ModelBaseRequest &base);
        static std::unique_ptr<MessageConvertible> fromMessage(const su::Text *pairs, const int pairsSize);
    };

    class ScanLedDeviceResponse final : public ModelBaseResponse
    {
    public:
        String name;
        ScanLedDeviceResponse() = delete;
        ScanLedDeviceResponse(const String &name, const ModelBaseResponse &base);
        ScanLedDeviceResponse(String &&name,  ModelBaseResponse &&base);
        String toMessage() const override;
    };

    /*===================================== GetLedDevice =========================================================*/

    class GetLedDeviceRequest final : public ModelBaseRequest
    {
    public:
        GetLedDeviceRequest() = delete;
        GetLedDeviceRequest(const ModelBaseRequest &base);
        GetLedDeviceRequest(ModelBaseRequest &&base);
        static std::unique_ptr<MessageConvertible> fromMessage(const su::Text *pairs, const int pairsSize);
    };

    class GetLedDeviceResponse final : public ModelBaseResponse
    {
    public:
        StringN<8> color;
        bool status;
        GetLedDeviceResponse() = delete;
        GetLedDeviceResponse(const char *color, const bool status, const ModelBaseResponse &base);
        GetLedDeviceResponse(const char *color, const bool status,  ModelBaseResponse &&base);
        String toMessage() const override;
    };
    /*===================================== ************ =========================================================*/
}

#endif // LED_DEVICE_MODELS_H