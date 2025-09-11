#pragma once
#include "ApiParse.h"

namespace api
{

    template <typename T>
    std::unique_ptr<ApiRequest> parseApiRequest(const char *message)
    {

        gson::Parser parser;
        parser.parse(message);

        if (parser.hasError())
        {
            String128 buf;
            buf.add(F("Api Parsing error: at index "));
            buf.add(parser.errorIndex());
            buf.add(F(": "));
            buf.add(parser.readError());
            return std::make_unique<ApiErrorRequest>(buf);
        }
        
        const gson::Entry &data = parser.get(F("data"));
        if (!data.valid())
        {   
            return std::make_unique<ApiErrorRequest>(String64(F("Api Parsing error: expected key 'data' not found")));
        }

        auto parsePtr = T::fromJson(data);
        if (!parsePtr->isOk())
        {
            JsonError *errorPtr = static_cast<JsonError *>(parsePtr.get());
            return std::make_unique<ApiErrorRequest>(errorPtr->message);
        }

        JsonParseSuccess<T> *successPtr = static_cast<JsonParseSuccess<T> *>(parsePtr.get());
        return std::make_unique<ApiSuccessRequest<T>>(std::move(successPtr->result));
    }
}
