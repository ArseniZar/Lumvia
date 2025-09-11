#pragma once
#ifndef MESSAGE_RESULT_H
#define MESSAGE_RESULT_H

#include <Arduino.h>
#include <StringN.h>

namespace telegram
{
    /*=====================MessageConvertible=========================*/

    struct MessageConvertible
    {
        virtual bool isOk() const = 0;
        virtual ~MessageConvertible() = default;
    };

    /*===================MessageParseSuccess===========================*/

    template <typename T>
    struct MessageParseSuccess final : public MessageConvertible
    {
        T result;
        MessageParseSuccess() = delete;
        MessageParseSuccess(const T &result) : result(result) {}
        MessageParseSuccess(T &&result) : result(std::move(result)) {}
        bool isOk() const override { return true; }
    };

    /*====================MessageError=================================*/

    struct MessageError final : public MessageConvertible
    {
        String128 message;
        MessageError() = delete;
        MessageError(const char *msg) : message(msg) {}
        bool isOk() const override { return false; }
    };

}

#endif // MESSAGE_RESULT_H