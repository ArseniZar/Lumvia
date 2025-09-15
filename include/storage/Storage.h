#pragma once
#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <FileData.h>
#include <LittleFS.h>
#include "StorageModel.h"

template <typename T>
class Storage
{

public:
    void begin();
    T readData();
    void writeData(T &data);
    
    Storage() = delete;
    Storage(const char *path, const uint8_t key);

private:
    T savedData;
    FileData file;
};

#include "Storage.tpp"

#endif // STORAGE_H