#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <ctime>

#include "Player.h"

Player::Player()
    : name(""), token(""), region("PL"),
      device(""), prefferedDeviceLanguage(""),
      osVersion(""), usedFreeNameChange(false),
      wins(0), losses(0), trophies(0), highestTrophies(0), trophyRoadTier(0),
      nameColor(0), thumbnail(0), coins(0), diamonds(0), xp(0),
      major(0), minor(0), build(0), id(0), registered(true)
{
}