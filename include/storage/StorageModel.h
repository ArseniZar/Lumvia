#pragma once
#ifndef STORAGEMODEL_H
#define STORAGEMODEL_H
#include <StringN.h>

struct SavedWifiData
{
    String32 ssid;
    String32 password;
    SavedWifiData() : ssid(""), password("") {}
    SavedWifiData(const char *ssid, const char *password) : ssid(ssid), password(password) {}
    SavedWifiData &operator=(const SavedWifiData &) = default;
};

#endif // STORAGEMODEL_H