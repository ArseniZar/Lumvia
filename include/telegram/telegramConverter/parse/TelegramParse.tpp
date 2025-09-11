#pragma once
#include "TelegramParse.h"

namespace telegram
{
    template <typename T>
    std::unique_ptr<TelegramRequest> parseTelegramRequest(const char *message)
    {
        su::Text parse(message);
        if (parse.length() < 3 || parse[0] != '/' || parse[parse.length() - 1] != '/')
        {
            return std::make_unique<TelegramErrorRequest>(String128(F("Telegram Parsing error: string either does not start/end with '/' or is correctly formatted but contains no data")));
        }

        su::Text content = parse.substring(1, parse.length() - 1);
        int pairCount = content.count(';');
        su::Text pairs[pairCount];
        content.split(pairs, pairCount, ';');
        auto parsePtr = T::fromMessage(pairs, pairCount);
        if (!parsePtr->isOk())
        {
            MessageError *errorPtr = static_cast<MessageError *>(parsePtr.get());
            return std::make_unique<TelegramErrorRequest>(errorPtr->message);
        }
        MessageParseSuccess<T> *successPtr = static_cast<MessageParseSuccess<T> *>(parsePtr.get());
        return std::make_unique<TelegramSuccessRequest<T>>(TelegramSuccessRequest<T>(std::move(successPtr->result)));
    }
}
