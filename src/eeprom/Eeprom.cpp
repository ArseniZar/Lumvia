#include "Eeprom.h"

Eeprom &Eeprom::init(const int size)
{
    static Eeprom instance(size);
    return instance;
}

Eeprom::Eeprom(const int size) : size(size) { EEPROM.begin(size); }

void Eeprom::writeString(const char *str, int startAddr)
{
    if (!str)
        return;

    int len = 0;
    while (str[len] != '\0')
        ++len;

    int maxLen = size - startAddr - 1;
    if (len > maxLen)
        len = maxLen;

    for (int i = 0; i < len; ++i)
    {
        EEPROM.write(startAddr + i, str[i]);
    }

    EEPROM.write(startAddr + len, 0);
    EEPROM.commit();
}

String Eeprom::readString(int startAddr)
{
    int maxLen = size - startAddr;
    char buf[maxLen + 1];

    int i = 0;
    for (; i < maxLen; ++i)
    {
        buf[i] = EEPROM.read(startAddr + i);
        if (buf[i] == 0)
            break;
    }

    buf[i] = 0;
    return String(buf);
}
