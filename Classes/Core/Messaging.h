#pragma once

#include <string>
#include <map>
#include <vector>

#include "../Wrappers/CallingInstance.h"

class Messaging
{
public:
    static void createMessage(int key, CallingInstance &callingInstance);
    static void handlePacket(int key, const std::vector<uint8_t> &messageData, CallingInstance &callingInstance);
};