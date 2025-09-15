#pragma once
#include "Storage.h"

template <typename T>
Storage<T>::Storage(const char *path, const uint8_t key) : file(&LittleFS, path, key, &savedData, sizeof(savedData)) {}

template <typename T>
void Storage<T>::begin()
{
    LittleFS.begin();
}

template <typename T>
T Storage<T>::readData()
{
    file.read();
    return savedData;
}

template <typename T>
void Storage<T>::writeData(T &data)
{
    savedData = data;
    file.updateNow();
}
