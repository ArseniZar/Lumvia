#include "JsonSerializationBase.h"

namespace api
{
    gson::Str JsonSerialization::serializationJson(std::tuple<const char *, gson::Type, String> *pairs, const int size)
    {
        gson::Str j;
        j('{');
        for (int i = 0; i < size; ++i)
        {
            const char *key = std::get<0>(pairs[i]);
            const gson::Type type = std::move(std::get<1>(pairs[i]));
            const String value = std::move(std::get<2>(pairs[i]));

            switch (type)
            {
            case gson::Type::Int:
                j[key] = value.toInt();
                break;

            case gson::Type::Float:
                j[key] = value.toFloat();
                break;

            case gson::Type::String:
                j[key] = value;
                break;

            case gson::Type::Bool:
            {
                bool result = value.equalsIgnoreCase("true");
                j[key] = result;
                break;
            }

            case gson::Type::Null:
                j[key] = nullptr;
                break;

            default:
                j[key] = value;
                break;
            }
        }
        j('}');
        return j;
    }
}