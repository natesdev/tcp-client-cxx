#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>

#include "Utility.h"
#include <random>

void Utility::parseFields(const std::map<std::string, std::string> &fields)
{
    for (const auto &field : fields)
    {
        std::cout << field.first << ": " << field.second << std::endl;
    }
}

void Utility::dumpPacket(const std::vector<uint8_t> &buffer, int packetID)
{
    std::ofstream file;
    file.open("DumpedPackets/" + std::to_string(packetID) + ".bin",
              std::ios::binary);
    file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
    file.close();
}

std::vector<uint8_t> Utility::getBytes(const std::string &input)
{
    return std::vector<uint8_t>(input.begin(), input.end());
}

int Utility::generateID()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    int id;
    for (int i = 0; i < 9; ++i)
    {
        id += dis(gen);
    }

    return id;
}

std::string Utility::generateToken()
{
    std::string token;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);

    const std::string charset =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<> char_dis(0, charset.size() - 1);
    for (int i = 0; i < 20; ++i)
    {
        token += charset[char_dis(gen)];
    }

    return token;
}
