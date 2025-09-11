#pragma once
#ifndef JSON_PARSE_BASE_H
#define JSON_PARSE_BASE_H

#include <Arduino.h>
#include <StringN.h>
#include <GSON.h>
#include <memory>
#include <map>
#include "JsonResult.h"

namespace api
{
    class JsonParse
    {
    public:
        virtual ~JsonParse() = default;

    protected:
        static std::unique_ptr<JsonConvertible> parseData(const gson::Entry &data, const std::map<const char *, gson::Type> &expectedKeys);
    };
}

#endif // JSON_PARSE_BASE_H