#include "MessageModelsBase.h"

namespace telegram
{
    namespace ModelsBaseRequestResponceKey
    {
        constexpr const char *COMMAND = "command";
        constexpr const char *ID = "id";
    }

    /* ================================================ModelBaseRequest================================================*/

    ModelBaseRequest::ModelBaseRequest(const ModelBaseRequest &base) : command(base.command), id(base.id) {}
    ModelBaseRequest::ModelBaseRequest(ModelBaseRequest &&base) : command(std::move(base.command)), id(base.id) {}
    ModelBaseRequest::ModelBaseRequest(const String &command, const char *id) : command(command), id(id) {}
    ModelBaseRequest::ModelBaseRequest(String &&command, const char *id) : command(std::move(command)), id(id) {}

    std::unique_ptr<MessageConvertible> ModelBaseRequest::fromMessage(const su::Text *pairs, const int pairsSize)
    {

        using namespace ModelsBaseRequestResponceKey;
        static const std::map<const char *, std::regex> keys = {
            {COMMAND, std::regex("^[a-z]+$")},
            {ID, std::regex("^([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$")}};

        auto parseBasePtr = MessageParse::parsePairs(pairs, pairsSize, keys);
        if (!parseBasePtr->isOk())
        {
            return parseBasePtr;
        }
        auto *successParseBasePtr = static_cast<MessageParseSuccess<std::map<const char *, String>> *>(parseBasePtr.get());
        std::map<const char *, String> map = std::move(successParseBasePtr->result);
        return std::make_unique<MessageParseSuccess<ModelBaseRequest>>(std::move(ModelBaseRequest(std::move(map[COMMAND]), map[ID].c_str())));
    }

    /* ================================================ModelBaseResponse================================================*/

    ModelBaseResponse::ModelBaseResponse(ModelBaseResponse &&base) : command(std::move(base.command)), id(base.id) {}
    ModelBaseResponse::ModelBaseResponse(ModelBaseRequest &&base) : command(std::move(base.command)), id(base.id) {}
    ModelBaseResponse::ModelBaseResponse(const ModelBaseResponse &base) : command(base.command), id(base.id) {}
    ModelBaseResponse::ModelBaseResponse(const ModelBaseRequest &base) : command(base.command), id(base.id) {}
    ModelBaseResponse::ModelBaseResponse(const String &command, const char *id) : command(command), id(id) {}
    ModelBaseResponse::ModelBaseResponse(String &&command, const char *id) : command(std::move(command)), id(id) {}

    String ModelBaseResponse::toMessage() const
    {   
        const int size = 2;
        using namespace ModelsBaseRequestResponceKey;
        std::tuple<const char *, String> pairs[size] = {
            {COMMAND, command},
            {ID, id.c_str()}};
        return MessageSerialization::serializationPairs(pairs, size);
    }
}