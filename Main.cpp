#include <iostream>
#include <unistd.h>
#include <thread>

#include "Classes/Core/Client.h"

bool debugLogging;

int main()
{
    Client client("127.0.0.1", 5005);

    client.connectToServer();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }

    return 0;
}
