#pragma once
#ifndef API_REQUEST_H
#define API_REQUEST_H

#include <Arduino.h>
#include <StringN.h>
#include "JsonParseBase.h"
#include "ApiTypesBase.h"
namespace api
{
    /*========================ApiSuccessRequest============================*/
    
    template <typename T>
    struct ApiSuccessRequest final : public ApiRequest
    {
        static_assert(std::is_base_of<JsonParse, T>::value, "T must inherit from JsonParse");

        T data;
        ApiSuccessRequest() = delete;
        ApiSuccessRequest(const T &data) : data(data) {}
        ApiSuccessRequest(T &&data) : data(std::move(data)) {}
        bool isOk() const override { return true; }
    };

    /*=========================ApiErrorRequest===========================*/

    struct ApiErrorRequest final : public ApiRequest
    {
        String256 message;
        ApiErrorRequest() = delete;
        ApiErrorRequest(const char *msg) : message(msg) {}
        bool isOk() const override { return false; }
    };
}
#endif // API_REQUEST_H