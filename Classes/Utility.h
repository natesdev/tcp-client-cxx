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
    static void parseFields(const std::map<std::string, std::string> &fields);
    static void dumpPacket(const std::vector<uint8_t> &buffer, int packetID);
    static bool isServerToClient(int messageType);
    static std::vector<uint8_t> getBytes(const std::string &str);
    static int generateID();
    static std::string generateToken();
};