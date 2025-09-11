#include "LedDeviceModels.h"

namespace telegram
{
    /*===================================== UpdateLedDevice =======================================================*/

    namespace UpdateLedDeviceRequestKey
    {
        constexpr const char *COLOR = "color";
        constexpr const char *STATUS = "status";
    }

    UpdateLedDeviceRequest::UpdateLedDeviceRequest(const char *color, const bool status, const ModelBaseRequest &base) : ModelBaseRequest(base), color(color), status(status) {}
    UpdateLedDeviceRequest::UpdateLedDeviceRequest(const char *color, const bool status, ModelBaseRequest &&base) : ModelBaseRequest(std::move(base)), color(color), status(status) {}

    std::unique_ptr<MessageConvertible> UpdateLedDeviceRequest::fromMessage(const su::Text *pairs, const int pairsSize)
    {

        using namespace UpdateLedDeviceRequestKey;
        static const std::map<const char *, std::regex> keys = {
            {COLOR, std::regex("^#[0-9A-Fa-f]{6}$")},
            {STATUS, std::regex("^(on|off)$")}};

        auto parseBasePtr = ModelBaseRequest::fromMessage(pairs, pairsSize);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        auto *successParseBasePtr = static_cast<MessageParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());

        auto parseUpdateLedDevicePtr = MessageParse::parsePairs(pairs, pairsSize, keys);
        if (!parseUpdateLedDevicePtr->isOk())
        {
            return parseUpdateLedDevicePtr;
        }
        auto *successParseUpdateLedDevicePtr = static_cast<MessageParseSuccess<std::map<const char *, String>> *>(parseUpdateLedDevicePtr.get());
        std::map<const char *, String> map = std::move(successParseUpdateLedDevicePtr->result);
        return std::make_unique<MessageParseSuccess<UpdateLedDeviceRequest>>(std::move(UpdateLedDeviceRequest(map[COLOR].c_str(), map[STATUS].equalsIgnoreCase("on"), successParseBasePtr->result)));
    }

    /*===================================== ScanLedDevice ==========================================================*/

    namespace ScanLedDeviceRequestKey
    {
    }
    
    ScanLedDeviceRequest::ScanLedDeviceRequest(ModelBaseRequest &&base) : ModelBaseRequest(std::move(base)) {}
    ScanLedDeviceRequest::ScanLedDeviceRequest(const ModelBaseRequest &base) : ModelBaseRequest(base) {}

    std::unique_ptr<MessageConvertible> ScanLedDeviceRequest::fromMessage(const su::Text *pairs, const int pairsSize)
    {

        using namespace ScanLedDeviceRequestKey;
        static const std::map<const char *, std::regex> keys = {};

        auto parseBasePtr = ModelBaseRequest::fromMessage(pairs, pairsSize);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        auto *successParseBasePtr = static_cast<MessageParseSuccess<ModelBaseRequest> *>(parseBasePtr.get());

        auto parseScanLedDevicePtr = MessageParse::parsePairs(pairs, pairsSize, keys);
        if (!parseScanLedDevicePtr->isOk())
        {
            return parseScanLedDevicePtr;
        }
        auto *successParseScanLedDevicePtr = static_cast<MessageParseSuccess<std::map<const char *, String>> *>(parseScanLedDevicePtr.get());
        std::map<const char *, String> _ = std::move(successParseScanLedDevicePtr->result);
        return std::make_unique<MessageParseSuccess<ScanLedDeviceRequest>>(std::move(ScanLedDeviceRequest(std::move(successParseBasePtr->result))));
    }

    namespace ScanLedDeviceResponseKey
    {
        constexpr const char *NAME = "name";
    }

    ScanLedDeviceResponse::ScanLedDeviceResponse(const String &name, const ModelBaseResponse &base) : ModelBaseResponse(base), name(name) {}
    ScanLedDeviceResponse::ScanLedDeviceResponse(String &&name, ModelBaseResponse &&base) : ModelBaseResponse(std::move(base)), name(std::move(name)) {}

    String ScanLedDeviceResponse::toMessage() const
    {
        const int size = 1;
        using namespace ScanLedDeviceResponseKey;
        std::tuple<const char *, String> pairs[size] = {
            {NAME, name}};

        String baseMessage = ModelBaseResponse::toMessage();
        String pairsMessage = MessageSerialization::serializationPairs(pairs, size);
        String result;
        result.reserve(baseMessage.length() + 1 + pairsMessage.length());

        result += std::move(baseMessage);
        result += ';';
        result += std::move(pairsMessage);
        return result;
    }

    /*========================================= other ==========================================================*/
}