#pragma once
#ifndef API_RESPONSE_H
#define API_RESPONSE_H

#include <Arduino.h>
#include <StringN.h>
#include "ApiTypesBase.h"
#include "JsonSerializationBase.h"
namespace api
{
    /*========================ApiSuccessResponse============================*/

    template <typename T>
    struct ApiSuccessResponse final : public ApiResponse
    {
        static_assert(std::is_base_of<JsonSerialization, T>::value, "T must inherit from JsonSerializable");

        int code;
        T data;
        ApiSuccessResponse() = delete;
        ApiSuccessResponse(const int code, const T &data) : code(code), data(data) {}
        ApiSuccessResponse(const int code, T &&data) : code(code), data(std::move(data)) {}
        bool isOk() const override { return true; }
        int getCode() const override { return code; }
    };

    /*=========================ApiErrorResponse===========================*/

    struct ApiErrorResponse final : public ApiResponse
    {
        int code;
        String256 message;
        ApiErrorResponse() = delete;
        ApiErrorResponse(const int code, const char *msg) : code(code), message(msg) {}
        bool isOk() const override { return false; }
        int getCode() const override { return code; };
    };
}
#endif // API_RESPONSE_H