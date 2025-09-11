#include "App.h"

App::App() : cachedSsid(""), cachedPass(""),
             logger(Logger::init(LOGGER_DEBUG_MODE)),
             eeprom(Eeprom::init(SIZE_EEPROM)),
             wifi(WiFiSetup::init(logger)),
             mac(MacAddress::init(wifi.getMAC())),
             bot(TelegramBot::init(logger, BOT_TOKEN, mac)),
             device(DeviceLed<NeoBrgFeature, NeoEsp8266Dma800KbpsMethod>(logger, mac, DEVICE_NAME, LED_COUNT, DEVICE_PIN))
{
    cachedSsid = eeprom.readString(ADDR_WIFI_SSID).c_str();
    cachedPass = eeprom.readString(ADDR_WIFI_PASS).c_str();
}

App &App::init()
{
    static App instance;
    return instance;
}

void App::begin()
{

#if ENABLE_DEVICE_MODULE
    device.begin();
#endif

#if ENABLE_WIFI_MODULE
    wifi.setMdnsName(MDNS_NAME);
    wifi.setAPConfig(AP_SSID, AP_PASS);
    wifi.setWiFiConfig(cachedSsid, cachedPass);
    wifi.begin();
#endif

#if ENABLE_WIFI_MODULE && ENABLE_TELEGRAM_BOT_MODULE
    if (wifi.status() == ConnState::WL_CONNECTED)
    {
        commitWiFiIfChanged();
#if ENABLE_TELEGRAM_BOT_MODULE

        bot.setLimitMessage(BOT_LIMIT);
        bot.setPeriodUpdate(BOT_PERIOD);
        bot.begin();
#if ENABLE_DEVICE_MODULE && ENABLE_TELEGRAM_BOT_MODULE
        bindDeviceToTelegramCommands();
#endif
#endif
    }

#endif
}

void App::update()
{
#if ENABLE_WIFI_MODULE
    if (wifi.status() != ConnState::WL_CONNECTED)
    {
        wifi.setWiFiConfig(cachedSsid, cachedPass);
        wifi.begin();
#if ENABLE_WIFI_MODULE
        if (wifi.status() == ConnState::WL_CONNECTED)
        {
            commitWiFiIfChanged();
        }
#endif
    }
#if ENABLE_TELEGRAM_BOT_MODULE
    else
    {
        bot.tick();
    }
#endif
#endif
}

void App::commitWiFiIfChanged()
{
    const char *currentSsid = wifi.getSsid();
    const char *currentPass = wifi.getPass();

    if (currentSsid != cachedSsid || currentPass != cachedPass)
    {
        eeprom.writeString(currentSsid, ADDR_WIFI_SSID);
        eeprom.writeString(currentPass, ADDR_WIFI_PASS);

        cachedSsid = currentSsid;
        cachedPass = currentPass;
    }
}

void App::bindDeviceToTelegramCommands()
{
    using namespace telegram;

    bot.registerCommand<ScanLedDeviceRequest, ScanLedDeviceResponse>(BOT_CMD_SCAN, [this](ScanLedDeviceRequest &request) -> ScanLedDeviceResponse
                                                                     { return ScanLedDeviceResponse(device.getName(), std::move(ModelBaseResponse(request.command, device.getMacAddress().getMac()))); });
    bot.registerCommand<UpdateLedDeviceRequest, void>(BOT_CMD_UPDATE, [this](UpdateLedDeviceRequest &request) -> void
        {device.setColor(request.color);
        device.setPower(request.status); });
}
