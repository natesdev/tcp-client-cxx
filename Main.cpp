#include "Classes/Core/Client.h"
#include "Config.h"

int main() 
{
    Client client(IP, PORT);
    client.connectToServer();
}
