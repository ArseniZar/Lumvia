#pragma once
#ifndef TELEGRAM_REQUEST_H
#define TELEGRAM_REQUEST_H

#include <Arduino.h>
#include <StringN.h>
#include "MessageParseBase.h"
#include "TelegramTypesBase.h"

namespace telegram
{
    /*========================TelegramSuccessRequest============================*/

    template <typename T>
    struct TelegramSuccessRequest final : public TelegramRequest
    {
        static_assert(std::is_base_of<MessageParse, T>::value, "T must inherit from MessageParse");

        T data;
        TelegramSuccessRequest() = delete;
        TelegramSuccessRequest(const T &data) : data(data) {}
        TelegramSuccessRequest(T &&data) : data(std::move(data)) {}
        bool isOk() const override { return true; }
    };

    /*========================TelegramErrorRequest===========================*/

    struct TelegramErrorRequest final : public TelegramRequest
    {
        String256 message;
        TelegramErrorRequest() = delete;
        TelegramErrorRequest(const char *msg) : message(msg) {}
        bool isOk() const override { return false; }
    };

}

#endif // TELEGRAM_REQUEST_H