#include "WiFiSetup.h"

WiFiSetup &WiFiSetup::init(Logger &logger)
{
    static WiFiSetup instance(logger);
    return instance;
}

WiFiSetup::WiFiSetup(Logger &logger)
    : logger(logger),
      webserver(logger, 80),
      apSsid(F("SmartHome")),
      apPassword(F("12345678")),
      ssid(F("")),
      password(F("")),
      mdnsName(F("smarthome"))
{
}

ConnState WiFiSetup::status()
{
    return static_cast<ConnState>(WiFi.status());
}

bool WiFiSetup::begin()
{
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(WiFiSetup::begin) Starting Wi-Fi setup..."); return buf; });

    if (attemptConnection(ssid, password))
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf = F("(WiFiSetup::begin) Wi-Fi setup finished successfully."); return buf; });
        return true;
    }

    if (!startAP(apSsid, apPassword))
    {
        logger.log(LOG_ERROR, [&]() -> String128
                   { String128 buf;  buf = F("(WiFiSetup::begin) Failed to start Access Point. Setup aborted."); return buf; });
        return false;
    }

    startMDNS(mdnsName);
    while (status() != ConnState::WL_CONNECTED || webserver.isRunning())
    {

        if (!webserver.isRunning())
            initServer();

        if (can_yield())
            esp_yield();

        webserver.handleClient();
        MDNS.update();
    }

    stopMDNS();
    stopAP();
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(WiFiSetup::begin) Wi-Fi setup finished successfully."); return buf; });

    return true;
}

void WiFiSetup::initServer()
{
    logger.log(LOG_INFO, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(WiFiSetup::initServer) Initializing web server..."));
                return buf; });

    webserver.begin([this]()
                    { return this->scanAvailableWiFiNetworks(); },
                    [this](const String32 &ssid, const String32 &pass)
                    { return this->attemptConnection(ssid, pass); });
}

bool WiFiSetup::attemptConnection(const char *ssid, const char *password)
{
    if (tryConnectWifi(ssid, password))
    {
        this->ssid = ssid;
        this->password = password;
        return true;
    }
    return false;
}

void WiFiSetup::setWiFiConfig(const char *ssid, const char *password)
{
    this->ssid = ssid;
    this->password = password;
}

void WiFiSetup::setAPConfig(const char *apSsid, const char *apPassword)
{
    this->apSsid = apSsid;
    this->apPassword = apPassword;
}

bool WiFiSetup::startAP(const String32 &apSsid, const String32 &apPassword)
{
    logger.log(LOG_INFO, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(WiFiSetup::startAP) Attempting to start Wi-Fi Access Point with SSID: '"));
                buf.add(apSsid);
                buf.add(F("'"));
                return buf; });

    if (apSsid.length() == 0)
    {
        logger.log(LOG_WARN, [&]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(WiFiSetup::startAP) Failed to start Access Point: SSID is empty."));
                    return buf; });
        return false;
    }

    WiFi.mode(WIFI_AP);

    bool phySet = WiFi.setPhyMode(WIFI_PHY_MODE_11G);
    logger.log(phySet ? LOG_DEBUG : LOG_WARN, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(WiFiSetup::startAP) PHY mode set to WIFI_PHY_MODE_11G with 50 ms delay "));
                buf.add(phySet ? F("enabled successfully") : F("failed to enable"));
                return buf; });

    bool apStarted = WiFi.softAP(apSsid, apPassword);
    logger.log(apStarted ? LOG_INFO : LOG_ERROR, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(WiFiSetup::startAP) Starting Wi-Fi Access Point "));
                buf.add(apStarted ? F("succeeded.") : F("failed. Please check configuration."));
                return buf; });

    if (!apStarted)
        return false;

    IPAddress apIP = WiFi.softAPIP();
    bool ipValid = (apIP != IPAddress(0, 0, 0, 0));
    logger.log(ipValid ? LOG_INFO : LOG_ERROR, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(WiFiSetup::startAP) AP IP address "));
                buf.add(ipValid ? F("is valid: ") : F("is invalid after starting softAP!"));
                if (ipValid) buf.add(apIP.toString().c_str());
                return buf; });

    if (!ipValid)
        return false;

    logger.log(LOG_INFO, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(WiFiSetup::startAP) Access Point started. SSID: "));
                buf.add(apSsid);
                buf.add(F(", Password: "));
                buf.add(apPassword);
                buf.add(F(", IP Address: "));
                buf.add(apIP.toString().c_str());
                return buf; });

    return true;
}

void WiFiSetup::setMdnsName(const char *mdnsName)
{
    this->mdnsName = mdnsName;
}

bool WiFiSetup::stopMDNS()
{
    bool status = MDNS.end();

    if (status)
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf.add(F("(WiFiSetup::stopMDNS) mDNS responder stopped successfully.")); return buf; });
    }
    else
    {
        logger.log(LOG_WARN, [&]() -> String128
                   { String128 buf; buf.add(F("(WiFiSetup::stopMDNS) Failed to stop mDNS responder.")); return buf; });
    }

    return status;
}

bool WiFiSetup::startMDNS(const String32 &mdnsName)
{
    bool status = MDNS.begin(mdnsName);
    if (status)
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf =  F("(WiFiSetup::startMDNS) mDNS responder started successfully with name: "); buf.add(mdnsName); return buf; });
        return status;
    }

    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf =  F("(WiFiSetup::startMDNS) Failed to start mDNS responder with name: "); buf.add(mdnsName); return buf; });

    return status;
}

bool WiFiSetup::stopAP()
{
    bool status = WiFi.softAPdisconnect(true);
    if (!status)
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf =  F("(WiFiSetup::stopAP) Wi-Fi Access Point stopped."); return buf; });
        return status;
    }

    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf =  F("(WiFiSetup::stopAP) Wi-Fi Access Point was not stopped."); return buf; });

    return status;
}

std::vector<api::Network> WiFiSetup::scanAvailableWiFiNetworks()
{
    logger.log(LOG_DEBUG, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(WiFiSetup::scanNetworks) Starting Wi-Fi scan..."));
                return buf; });

    int networksFound = WiFi.scanNetworks();
    if (networksFound <= 0)
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(WiFiSetup::scanNetworks) No Wi-Fi networks found."));
                    return buf; });
        return {};
    }

    logger.log(LOG_DEBUG, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(WiFiSetup::scanNetworks) Found "));
                buf.add(networksFound);
                buf.add(F(" Wi-Fi networks."));
                return buf; });

    std::vector<api::Network> networks;
    networks.reserve(networksFound);

    for (int i = 0; i < networksFound; i++)
    {
        api::Network newNetwork(
            WiFi.SSID(i).c_str(),
            "",
            WiFi.RSSI(i),
            WiFi.encryptionType(i),
            WiFi.channel(i),
            WiFi.BSSIDstr(i).c_str(),
            WiFi.isHidden(i));

        networks.push_back(std::move(newNetwork));

        logger.log(LOG_DEBUG, [&i, &networks]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(WiFiSetup::scanNetworks) SSID: "));
                    buf.add(networks[i].ssid);
                    buf.add(F(", RSSI: "));
                    buf.add(networks[i].rssi);
                    buf.add(F(", EncryptionType: "));
                    buf.add(networks[i].encryptionType);
                    buf.add(F(", Channel: "));
                    buf.add(networks[i].channel);
                    buf.add(F(", BSSID: "));
                    buf.add(networks[i].bssid);
                    buf.add(F(", Hidden: "));
                    buf.add(networks[i].hidden ? F("true") : F("false"));
                    return buf; });
    }

    WiFi.disconnect(true);
    return networks;
}

StringN<18> WiFiSetup::getMAC() const
{
    return WiFi.macAddress().c_str();
}

const char *WiFiSetup::getSsid() const
{
    return ssid;
}

const char *WiFiSetup::getPass() const
{
    return password;
}

bool WiFiSetup::tryConnectWifi(const String32 &ssid, const String32 &password)
{
    logger.log(LOG_INFO, [&]() -> String128
               {
                String128 buf;
                buf.add(F("(WiFiSetup::tryConnectWifi) Attempting to connect to SSID: '"));
                buf.add(ssid);
                buf.add(F("'  PASS: '"));
                buf.add(password);
                buf.add(F("'"));
                return buf; });

    if (ssid.length() == 0)
    {
        logger.log(LOG_WARN, [&]() -> String128
                   { String128 buf; buf.add(F("(WiFiSetup::tryConnectWifi) Failed to connect to Wi-Fi network: SSID is empty.")); return buf; });
        return false;
    }

    WiFi.begin(ssid, password);
    const unsigned long startTime = millis();

    while (status() != ConnState::WL_CONNECTED && millis() - startTime < WIFI_CONNECTION_TIMEOUT_MS)
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf.add(F("(WiFiSetup::tryConnectWifi) Connecting to Wi-Fi...")); return buf; });
        delay(500);
    }

    if (status() == ConnState::WL_CONNECTED)
    {
        logger.log(LOG_INFO, [&]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(WiFiSetup::tryConnectWifi) Successfully connected to Wi-Fi network: '"));
                    buf.add(ssid);
                    buf.add(F("' with IP: "));
                    buf.add(WiFi.localIP().toString().c_str());
                    return buf; });

        bool sleepModeSet = WiFi.setSleepMode(WIFI_NONE_SLEEP, 50);
        logger.log(sleepModeSet ? LOG_DEBUG : LOG_WARN, [&]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(WiFiSetup::tryConnectWifi) Wi-Fi sleep mode WIFI_NONE_SLEEP with 50 ms delay "));
                    buf.add(sleepModeSet ? F("enabled successfully") : F("failed to enable"));
                    return buf; });

        WiFi.setOutputPower(20.5f);
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf.add(F("(WiFiSetup::tryConnectWifi) Wi-Fi output power set to 20.5 dBm")); return buf; });
        return true;
    }
    else
    {
        logger.log(LOG_WARN, [&]() -> String128
                   {
                    String128 buf;
                    buf.add(F("(WiFiSetup::tryConnectWifi) Failed to connect to Wi-Fi network: '"));
                    buf.add(ssid);
                    buf.add(F("'"));
                    return buf; });

        WiFi.disconnect(true);
        return false;
    }
}
