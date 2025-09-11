#pragma once
#ifndef JSON_RESULT_H
#define JSON_RESULT_H

#include <Arduino.h>

namespace api
{
    /*=====================JsonConvertible=========================*/

    struct JsonConvertible
    {
        virtual bool isOk() const = 0;
        virtual ~JsonConvertible() = default;
    };

    /*===================JsonParseSuccess===========================*/

    template <typename T>
    struct JsonParseSuccess final : public JsonConvertible
    {
        T result;
        JsonParseSuccess() = delete;
        JsonParseSuccess(const T &result) : result(result) {}
        JsonParseSuccess(T &&result): result(std::move(result)){}
        bool isOk() const override { return true; }
    };

    /*====================JsonError=================================*/

    struct JsonError final : public JsonConvertible
    {
        String128 message;
        JsonError() = delete;
        JsonError(const char *msg) : message(msg) {}
        bool isOk() const override { return false; }
    };

}

#endif // JSON_RESULT_H