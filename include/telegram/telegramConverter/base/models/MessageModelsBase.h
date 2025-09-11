#pragma once
#ifndef MESSAGE_MODELS_BASE_H
#define MESSAGE_MODELS_BASE_H

#include <Arduino.h>
#include "MessageParseBase.h"
#include "MessageSerializationBase.h"

namespace telegram
{
    /*==========================================================ModelRequest===============================================*/

    class ModelBaseRequest : public MessageParse
    {
    public:
        String command;
        StringN<18> id; 
        ModelBaseRequest() = delete;
        ModelBaseRequest(ModelBaseRequest &&base);
        ModelBaseRequest(const ModelBaseRequest &base);
        ModelBaseRequest(const String &command, const char *id);
        ModelBaseRequest(String &&command, const char *id);
        static std::unique_ptr<MessageConvertible> fromMessage(const su::Text *pairs, const int pairsSize);
    };

    /*==========================================================ModelResponse==============================================*/

    class ModelBaseResponse : public MessageSerialization
    {
    public:
        String command;
        StringN<18> id;
        ModelBaseResponse() = delete;
        ModelBaseResponse(ModelBaseResponse &&base);
        ModelBaseResponse(ModelBaseRequest &&base);
        ModelBaseResponse(const ModelBaseResponse &base);
        ModelBaseResponse(const ModelBaseRequest &base);
        ModelBaseResponse(const String &command, const char *id);
        ModelBaseResponse(String &&command, const char *id);
        String toMessage() const override;
    };
}

#endif // MESSAGE_MODELS_BASE_H