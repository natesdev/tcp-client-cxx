#pragma once

#include <string>

#include "Player.h"

struct SetPlayerNameData
{
    std::string name;
};

struct CallingInstance
{
    Player player;
    int socket;

    // Packet specific data
    SetPlayerNameData setPlayerNameData;
};