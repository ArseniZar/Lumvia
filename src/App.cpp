#include "App.h"

App::App() : savedWifiData(),
             storage(Storage<SavedWifiData>("/data.dat", 'W')),
             logger(Logger::init(LOGGER_DEBUG_MODE)),
             wifi(WiFiSetup::init(logger)),
             mac(MacAddress::init(wifi.getMAC())),
             bot(TelegramBot::init(logger, BOT_TOKEN, mac)),
             device(DeviceLed<NeoBrgFeature, NeoEsp8266Dma800KbpsMethod>(logger, mac, DEVICE_NAME, LED_COUNT, DEVICE_PIN))
{
}

App &App::init()
{
    static App instance;
    return instance;
}

void App::begin()
{
    
#if ENABLE_STORAGE_MODULE
    storage.begin();
    savedWifiData = storage.readData();
#endif

#if ENABLE_DEVICE_MODULE
    device.begin();
#endif

#if ENABLE_WIFI_MODULE
    wifi.setMdnsName(MDNS_NAME);
    wifi.setAPConfig(AP_SSID, AP_PASS);
    wifi.setWiFiConfig(savedWifiData.ssid, savedWifiData.password);
    wifi.begin();
#endif

#if ENABLE_WIFI_MODULE && ENABLE_TELEGRAM_BOT_MODULE
    if (wifi.statusWifi() == ConnState::WL_CONNECTED)
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
    if (wifi.statusWifi() != ConnState::WL_CONNECTED)
    {
        wifi.setWiFiConfig(savedWifiData.ssid, savedWifiData.password);
        wifi.begin();
#if ENABLE_WIFI_MODULE
        if (wifi.statusWifi() == ConnState::WL_CONNECTED)
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
    String32 currentSsid = wifi.getSsid();
    String32 currentPass = wifi.getPass();

    if (currentSsid != savedWifiData.ssid || currentPass != savedWifiData.password)
    {
        savedWifiData.ssid = currentSsid;
        savedWifiData.password = currentPass;
#if ENABLE_STORAGE_MODULE
        storage.writeData(savedWifiData);
#endif
    }
}

void App::bindDeviceToTelegramCommands()
{
    using namespace telegram;

    bot.registerCommand<ScanLedDeviceRequest, ScanLedDeviceResponse>(BOT_CMD_SCAN, [this](ScanLedDeviceRequest &request) -> ScanLedDeviceResponse
        { 
            return ScanLedDeviceResponse(device.getName(), std::move(ModelBaseResponse(request.command, device.getMacAddress().getMac()))); 
        }
    );
    bot.registerCommand<UpdateLedDeviceRequest, void>(BOT_CMD_UPDATE, [this](UpdateLedDeviceRequest &request) -> void
        {
            device.setColor(request.color); device.setPower(request.status); 
        }
    );
    bot.registerCommand<GetLedDeviceRequest, GetLedDeviceResponse>(BOT_CMD_GET,[this](GetLedDeviceRequest& request) -> GetLedDeviceResponse
        {
            return GetLedDeviceResponse(device.getColor(),device.getStatus(),std::move(ModelBaseResponse(request.command,device.getMacAddress().getMac())));
        }
    );
}