#include "JsonParseBase.h"

namespace api
{
    std::unique_ptr<JsonConvertible> JsonParse::parseData(const gson::Entry &data, const std::map<const char *, gson::Type> &expectedKeys)
    {
        struct Key
        {
            const char *keyPtr;
            Key() = delete;
            Key(const char *keyPtr) { this->keyPtr = keyPtr; }
            bool operator<(const Key &other) const
            {
                return strcmp(keyPtr, other.keyPtr) < 0;
            }
            const char *c_str() const
            {
                return keyPtr;
            }
        };

        if (!data.is(gson::Type::Object))
        {
            return std::make_unique<JsonError>(String64(F("Json Parse error: data is not a JSON object")));
        }

        std::map<Key, String> map;
        for (const auto &[key, _] : expectedKeys)
        {
            map[Key(key)] = "#";
        }

        if (expectedKeys.size() > 0)
        {
            for (int i = 0; i < (int)data.length(); ++i)
            {
                const gson::Entry &pair = data.get(i);
                if (!pair.isContainer())
                {
                    String key(pair.key().c_str());
                    String value(pair.value().c_str());

                    auto it = map.find(Key(key.c_str()));
                    if (it != map.end())
                    {
                        if (it->second.equalsIgnoreCase(F("#")))
                        {
                            if (!pair.is(expectedKeys.at(it->first.c_str())))
                            {
                                String128 buf;
                                buf.add(F("Json Parse error: "));
                                buf.add(F("Value for key '"));
                                buf.add(key.c_str());
                                buf.add(F("' does not match expected type"));
                                return std::make_unique<JsonError>(buf);
                            }

                            it->second = std::move(value);
                        }
                        else
                        {
                            String128 buf;
                            buf.add(F("Json Parse error: "));
                            buf.add(F("Duplicate key '"));
                            buf.add(key.c_str());
                            buf.add(F("' at index "));
                            buf.add(i);
                            buf.add(F(": previous value='"));
                            buf.add(it->second.c_str());
                            buf.add(F("', new value='"));
                            buf.add(value.c_str());
                            buf.add(F("'"));
                            return std::make_unique<JsonError>(buf);
                        }
                    }
                    else
                    {
                        // Неизвестный ключ (если нужно, можно раскомментировать)
                        // Serial.print(F("Unknown key: "));
                        // Serial.println(key);
                    }
                }
            }
        }

        std::map<const char *, String> outMap;
        String128 buf;

        for (const auto &[key, value] : map)
        {
            if (value.equalsIgnoreCase(F("#")))
            {
                if (buf.length() == 0)
                {
                    buf.add(F("Json Parse error: "));
                    buf.add(F("Missing value for expected key: "));
                }
                buf.add(F("'"));
                buf.add(key.c_str());
                buf.add(F("' "));
            }

            outMap[key.c_str()] = std::move(value);
        }

        if (buf.length() > 0)
        {
            return std::make_unique<JsonError>(buf);
        }

        return std::make_unique<JsonParseSuccess<std::map<const char *, String>>>(std::move(outMap));
    }
}