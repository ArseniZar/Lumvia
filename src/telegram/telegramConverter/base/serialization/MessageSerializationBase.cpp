#include "MessageSerializationBase.h"
namespace telegram
{
    String MessageSerialization::serializationPairs(std::tuple<const char *, String> *pairs, const int pairsSize)
    {
        int totalLen = 0;
        for (int i = 0; i < pairsSize; ++i)
        {

            totalLen += strlen(std::get<0>(pairs[i]));
            totalLen += std::get<1>(pairs[i]).length();
            totalLen += 1;
            if (i < pairsSize - 1)
                totalLen += 1;
        }

        String result;
        result.reserve(totalLen + 1);

        for (int i = 0; i < pairsSize; ++i)
        {
            result += std::get<0>(pairs[i]);
            result += '=';
            result += std::move(std::get<1>(pairs[i]));
            if (i < pairsSize - 1)
                result += ';';
        }

        return result;
    }
}