#pragma once
#ifndef MESSAGE_SERIALIZATION_BASE_H
#define MESSAGE_SERIALIZATION_BASE_H

#include <Arduino.h>
#include <vector>
#include <tuple>
#include <StringUtils.h>

namespace telegram
{
    class MessageSerialization
    {
    public:
        virtual ~MessageSerialization() = default;
        virtual String toMessage() const = 0;

    protected:
        static String serializationPairs(std::tuple<const char *, String> *pairs, const int pairsSize);
    };
}

#endif // MESSAGE_SERIALIZATION_BASE_H