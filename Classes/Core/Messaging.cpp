#include "Messaging.h"
#include "../Core/Logger.h"
#include "../Wrappers/CallingInstance.h"
#include "../Utility.h"

void Messaging::createMessage(int key, CallingInstance &callingInstance)
{
    Logger logger("Messaging");
    switch (key)
    {
        logger.Warn("createMessage", "Unknown packet id: " + std::to_string(key));
    }
}

void Messaging::handlePacket(int key, const std::vector<uint8_t> &messageData, CallingInstance &callingInstance)
{
    Logger logger("Messaging");

    switch (key)
    {
    default:
        logger.Warn("handlePacket", "Packet " + std::to_string(key) + " is not handled");
        Utility::dumpPacket(messageData, key);
        break;
    }
}
