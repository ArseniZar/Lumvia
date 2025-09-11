#pragma once
#include "TelegramSerialization.h"

namespace telegram
{
    template <typename T>
    String serializationTelegramResponce(const TelegramSuccessResponse<T> &responce)
    {
        String message = responce.data.toMessage();
        String result;
        result.reserve(message.length() + 2);
        result += '/';
        result += std::move(message);
        result += '/';
        return result;
    }
}