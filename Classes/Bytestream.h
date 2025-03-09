#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>
#include <string>

#include "Logger.h"

extern Logger logger;

class Bytestream
{
public:
    Bytestream(const std::vector<uint8_t> &messageData);
    Bytestream();
    std::vector<uint8_t> messagePayload;
    size_t bitoffset;
    size_t offset;
    size_t length;
    int readInt();
    int readInt8();
    int readInt16();
    int readInt24();
    int readIntLittleEndian(); // Same as Big Endian, but the bytes are reversed.
    long readLong();
    long long readLongLong();
    int readShort();
    std::string readString();
    bool readBoolean();
    std::vector<uint8_t> readBytes(size_t length);
    std::string readCompressedString();
    void writeInt8(int value);
    void writeInt16(int value);
    void writeInt24(int value);
    void writeInt(int value);
    void writeIntLittleEndian(int value);
    void writeInty(int value);
    void writeLong(int high, int low);
    void writeShort(int value);
    void writeString(const std::string &value);
    void writeStringReference(const std::string &value);
    void writeBoolean(bool value);
    void writeDataReference(int high = 0, int low = -1);
    void writeCompressedString(const std::string &data);
    void writeByte(uint8_t value);
    void writeBytes(const uint8_t value, size_t length);
    void writeHexa(const std::string &data, size_t length);
    void writePacketHeader(int packetID);
};
