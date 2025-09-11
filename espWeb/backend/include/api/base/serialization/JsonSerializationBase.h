#pragma once
#ifndef JSON_SERIALIZATION_BASE_H
#define JSON_SERIALIZATION_BASE_H

#include <Arduino.h>
#include <GSON.h>
#include <utility>

namespace api
{
    class JsonSerialization
    {
    public:
        virtual ~JsonSerialization() = default;
        virtual gson::Str toJson() const = 0;

    protected:
        static gson::Str serializationJson(std::tuple<const char *, gson::Type, String> *pairs, const int size);
    };
}

#endif // JSON_SERIALIZATION_BASE_H