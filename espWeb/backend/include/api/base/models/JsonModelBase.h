#pragma once
#ifndef JSON_MODELS_BASE_H
#define JSON_MODELS_BASE_H

#include <Arduino.h>
#include "JsonParseBase.h"
#include "JsonSerializationBase.h"

namespace api
{
    /*==========================================================ModelRequest===============================================*/

    class ModelBaseRequest : public JsonParse
    {
    public:
        ModelBaseRequest();
        ModelBaseRequest(ModelBaseRequest &&base);
        ModelBaseRequest(const ModelBaseRequest &base);
        static std::unique_ptr<JsonConvertible> fromJson(const gson::Entry &data);

        // const String32 meta;
        // ModelBaseRequest() = delete;
        // ModelBaseRequest(const char *meta);
    };

    /*==========================================================ModelResponse==============================================*/

    class ModelBaseResponse : public JsonSerialization
    {
    public:
        ModelBaseResponse();
        ModelBaseResponse(ModelBaseResponse &&base);
        ModelBaseResponse(ModelBaseRequest &&base);
        ModelBaseResponse(const ModelBaseResponse &base);
        ModelBaseResponse(const ModelBaseRequest &base);
        gson::Str toJson() const override;

        // const String32 meta;
        // ModelBaseResponse() = delete;
        // ModelBaseResponse(const char *meta);
    };
}

#endif // JSON_MODELS_BASE_H