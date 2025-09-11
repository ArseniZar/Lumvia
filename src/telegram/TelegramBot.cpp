#include "TelegramBot.h"

TelegramBot::TelegramBot(Logger &logger, const char *token, const MacAddress &mac) : logger(logger), bot(FastBot2(token)), mac(mac), limitMessage(10), periodUpdate(1000) {}

TelegramBot &TelegramBot::init(Logger &logger, const char *token, const MacAddress &mac)
{
    static TelegramBot instance(logger, token, mac);
    return instance;
}

void TelegramBot::begin()
{
    bot.setLimit(limitMessage);
    bot.setPollMode(fb::Poll::Sync, periodUpdate);
    bot.updates.set(fb::Updates::Type::Message | fb::Updates::Type::ChannelPost);
    bot.skipUpdates();
    bot.onUpdate([this](fb::Update &u)
                 { this->handleUpdateMsg(u); });
}
void TelegramBot::handleUpdateMsg(fb::Update &u)
{
    auto parseBasePtr = telegram::parseTelegramRequest<telegram::ModelBaseRequest>(u.message().text().c_str());
    if (parseBasePtr->isOk())
    {
        auto *successParseBasePtr = static_cast<telegram::TelegramSuccessRequest<telegram::ModelBaseRequest> *>(parseBasePtr.get());
        String parseCommand = std::move(successParseBasePtr->data.command);
        StringN<18> parseId = successParseBasePtr->data.id;
        Serial.println(parseId);
        logger.log(LOG_DEBUG, [&]() -> String256
                   { String256 buf;
                     buf.add(F("[TelegramBot::handleUpdateMsg] Successfully parsed ModelBaseRequest: command="));
                     buf.add(parseCommand.c_str());
                     buf.add(F(", id="));
                     buf.add(parseId);
                    return buf; });

        if (mac.equals(parseId) || mac.isBroadcast(parseId))
        {
            auto it = handlers.find(parseCommand);
            if (it != handlers.end())
            {
                it->second(u);
            }
            else
            {
                logger.log(LOG_DEBUG, [&]() -> String256
                           { String256 buf;
                     buf.add(F("[TelegramBot::handleUpdateMsg] Command not implemented: "));
                     buf.add(parseCommand.c_str());
                    return buf; });
            }
        }
        else
        {
            logger.log(LOG_DEBUG, [&]() -> String256
                       { String256 buf;
                     buf.add(F("[TelegramBot::handleUpdateMsg] Received command '"));
                     buf.add(parseCommand.c_str());
                     buf.add(F("' not intended for this device (MAC: "));
                     buf.add(mac.getMac());
                     buf.add(F(", target ID: "));
                     buf.add(parseId);
                     buf.add(')');
                    return buf; });
        }
    }
    else
    {
        auto *errorParseBasePtr = static_cast<telegram::TelegramErrorRequest *>(parseBasePtr.get());

        logger.log(LOG_WARN, [&]() -> String256
                   { String256 buf;
                     buf.add(F("[TelegramBot::handleUpdateMsg] Failed to parse ModelBaseRequest: "));
                     buf.add(u.message().text().c_str());
                     buf.add(F(" nError: "));
                     buf.add(errorParseBasePtr->message);
                    return buf; });
    }
}

void TelegramBot::setLimitMessage(int limitMessage)
{
    this->limitMessage = limitMessage;
    bot.setLimit(limitMessage);
}

void TelegramBot::setPeriodUpdate(int periodUpdate)
{
    this->periodUpdate = periodUpdate;
    bot.setPollMode(fb::Poll::Sync, periodUpdate);
}

bool TelegramBot::tick()
{

    return bot.tick();
}