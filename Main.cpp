#include <iostream>
#include <unistd.h>
#include <thread>

#include "Main.h"
#include "Classes/Core/Client.h"
#include "Classes/Core/Logger.h"

bool debugLogging;

int main()
{
    Client client("127.0.0.1", 5005);
    Logger logger("main");
    debugLogging = true;

    client.connectToServer();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }

    return 0;
}
