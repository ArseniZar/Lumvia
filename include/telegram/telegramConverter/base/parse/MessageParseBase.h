#pragma once
#ifndef MESSAGE_PARSE_BASE_H
#define MESSAGE_PARSE_BASE_H

#include <Arduino.h>
#include <memory>
#include <map>
#include <regex>
#include <StringUtils.h>
#include "MessageResult.h"

namespace telegram
{
    class MessageParse
    {
    public:
        virtual ~MessageParse() = default;

    protected:
        static std::unique_ptr<MessageConvertible> parsePairs(const su::Text *pairs, const int pairsSize, const std::map<const char *, std::regex> &expectedKeys);
    };
}
#endif // MESSAGE_PARSE_BASE_H
