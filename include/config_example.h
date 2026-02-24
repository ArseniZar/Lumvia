#pragma once
// Copy this file to config.h and fill in values for your device.
// config.h should NOT be committed to git (see .gitignore).

/* ====================== WIFI CONFIG ====================== */

// Access point used when the device creates its own Wi‑Fi network
// Example: "SmartHome"
#define AP_SSID ""

// Password for the access point (minimum 8 characters)
// Example: "12345678"
#define AP_PASS ""

// mDNS name to find the device in the network: <MDNS_NAME>.local
// Must be lowercase, e.g. "smarthome" → smarthome.local
#define MDNS_NAME ""

/* ====================== TELEGRAM BOT CONFIG ====================== */

// Telegram bot token from @BotFather
// Example: "123456789:AA..."
#define BOT_TOKEN ""

// Maximum messages processed per tick (anti‑spam / rate limiting)
#define BOT_LIMIT 10

// Bot update period in milliseconds
// Example: 100 = check updates every 100 ms
#define BOT_PERIOD 100

/* ====================== DEVICE CONFIG ====================== */

// Data pin for LED strip (board pin number, e.g. D5)
// For NodeMCU D5 corresponds to GPIO14 → value 14 or appropriate mapping
#define DEVICE_PIN 3

// Number of LEDs in your strip or matrix
// Example: 40
#define LED_COUNT 40

// Logical device name shown in UI / Telegram
// Example: "Led", "KitchenStrip", "TVBacklight"
#define DEVICE_NAME ""
