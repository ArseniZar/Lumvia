#include "TelegramSerialization.h"

namespace telegram
{
    String serializationTelegramResponce(const TelegramErrorResponse &response)
    {
        String result;
        result.reserve(response.message.length() + 2);
        result += '/';
        result += response.message.c_str();
        result += '/';
        return result;
    }
}