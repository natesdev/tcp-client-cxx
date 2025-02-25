#pragma once

#include <vector>
#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include <array>

class Utility
{
public:
    static void dumpPacket(const std::vector<uint8_t> &buffer, int packetID);
};