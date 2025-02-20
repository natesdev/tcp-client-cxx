#pragma once

#include <iostream>
#include <vector>
#include <map>

class Player
{
public:
    std::string name;
    std::string token;
    int id;
    bool registered, usedFreeNameChange;
    int nameColor, thumbnail, coins, diamonds, wins, losses, xp, major, minor, build, trophies, highestTrophies, trophyRoadTier;
    std::string region;
    std::string device, prefferedDeviceLanguage, osVersion;
    Player();
};