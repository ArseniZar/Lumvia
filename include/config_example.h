#pragma once
/* ======================CONFIG WIFI======================*/
#define AP_SSID ""   // Данные точки доступа пример: smartHome
#define AP_PASS ""    // Данные точки доступа пример: 12345678
#define MDNS_NAME "" // ${MDNS_NAME}.local имя под которомы можно найти устрйоство в сети. !! Обязательно маленькими буквами

/* ======================CONFIG BOT======================*/
#define BOT_TOKEN ""
#define BOT_LIMIT 10
#define BOT_PERIOD 100

#define BOT_CMD_SCAN "scan"
#define BOT_CMD_UPDATE "update"
#define BOT_CMD_GET "get"

/*=======================CONFIG DEVICE==================*/
#define DEVICE_PIN 3 // должен быть D5 
#define LED_COUNT 40
#define DEVICE_NAME "" // Led
