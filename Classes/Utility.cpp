#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>

#include "Utility.h"
#include <random>

void Utility::dumpPacket(const std::vector<uint8_t> &buffer, int packetID)
{
    std::ofstream file;
    file.open("DumpedPackets/" + std::to_string(packetID) + ".bin",
              std::ios::binary);
    file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
    file.close();
}