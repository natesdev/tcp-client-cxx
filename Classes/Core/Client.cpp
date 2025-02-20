#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <ctime>
#include <thread>

#include "Client.h"
#include "../Wrappers/CallingInstance.h"
#include "../Core/Messaging.h"

Client::Client(const std::string &addr, int port) : addr(addr), port(port)
{
}


void Client::connectToServer()
{
    callingInstance.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (callingInstance.socket == -1)
    {
        logger.Error("connectToServer", "Failed to create socket");
        return;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, addr.c_str(), &serverAddress.sin_addr);

    if (connect(callingInstance.socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        logger.Error("connectToServer", "Failed to connect to server");
        close(callingInstance.socket);
        return;
    }

    logger.Info("connectToServer", "Connected to server");

    std::thread(&Client::receiveMessages, this, std::ref(callingInstance)).detach();
}

void Client::receiveMessages(CallingInstance &callingInstance)
{
    while (true)
    {
        char buffer[1024];
        int bytesRead = recv(callingInstance.socket, buffer, sizeof(buffer), 0);
        if (bytesRead >= 7)
        {
            int packetId = (buffer[0] << 8) | (buffer[1] & 0xFF);
            int dataLength = (buffer[2] << 16) | (buffer[3] << 8) | (buffer[4] & 0xFF);

            if (bytesRead >= dataLength + 7)
            {
                std::vector<uint8_t> message(buffer + 7, buffer + 7 + dataLength);
                Messaging::handlePacket(packetId, message, callingInstance);
            }
            else
            {
                logger.Error("receiveMessages", "Incomplete packet received");
            }
        }
        else if (bytesRead == 0)
        {
            logger.Info("receiveMessages", "Server disconnected");
            close(callingInstance.socket);
            break;
        }
        else
        {
            close(callingInstance.socket);
            break;
        }
    }
}