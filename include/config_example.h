#pragma once
/* ======================CONFIG WIFI======================*/
#define AP_SSID ""   // Access point SSID, e.g. smartHome
#define AP_PASS ""   // Access point password, e.g. 12345678
#define MDNS_NAME "" // ${MDNS_NAME}.local name to find the device on the network (must be lowercase)

/* ======================CONFIG BOT======================*/
#define BOT_TOKEN ""
#define BOT_LIMIT 10
#define BOT_PERIOD 100

#define BOT_CMD_SCAN "scan"
#define BOT_CMD_UPDATE "update"
#define BOT_CMD_GET "get"

/*=======================CONFIG DEVICE==================*/
#define DEVICE_PIN 3 // Must be D5
#define LED_COUNT 40
#define DEVICE_NAME "" // Device name, e.g. Led
