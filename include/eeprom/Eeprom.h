#pragma once
#ifndef EEPROMHELPER_H
#define EEPROMHELPER_H

#include <EEPROM.h>
#include <Arduino.h>

class Eeprom
{
public:
    String readString(int startAddr = 0);
    void writeString(const char *, int startAddr = 0);
    static Eeprom &init(const int size);

private:
    const int size;
    Eeprom() = delete;
    Eeprom(const int size);
};

#endif // EEPROMHELPER_H
