#pragma once

#include <string>

#include "Logger.h"
#include "../Wrappers/CallingInstance.h"

class Client
{
public:
    Client(const std::string &addr, int port);
    const int port;
    const std::string addr;
    Logger logger = Logger("Client");
    void receiveMessages(CallingInstance &callingInstance);
    void connectToServer();
    void listenForConnections();
private:
    CallingInstance callingInstance;
};