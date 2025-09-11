#pragma once
#ifndef TELEGRAM_RESPONSE_H
#define TELEGRAM_RESPONSE_H

#include <Arduino.h>
#include <StringN.h>
#include "MessageSerializationBase.h"
#include "TelegramTypesBase.h"

namespace telegram
{
    /*==========================TelegramSuccessResponse============================*/

    template <typename T>
    struct TelegramSuccessResponse final : public TelegramResponse
    {
        static_assert(std::is_base_of<MessageSerialization, T>::value, "T must inherit from MessageSerialization");

        T data;
        TelegramSuccessResponse() = delete;
        TelegramSuccessResponse(const T &data) : data(data) {}
        TelegramSuccessResponse(T &&data) : data(std::move(data)) {}
        bool isOk() const override { return true; }
    };

    /*============================TelegramErrorResponse===========================*/

    struct TelegramErrorResponse final : public TelegramResponse
    {
        String256 message;
        TelegramErrorResponse() = delete;
        TelegramErrorResponse(const char *msg) : message(msg) {}
        bool isOk() const override { return false; }
    };

}

#endif // TELEGRAM_RESPONSE_H