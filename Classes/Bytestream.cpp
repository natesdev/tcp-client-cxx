#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>
#include <zlib.h>

#include "Bytestream.h"
// #include "ChecksumEncoder.h"
#include "Logger.h"

Logger logger("Bytestream");

Bytestream::Bytestream(const std::vector<uint8_t> &messageData)
    : bitoffset(0), offset(0), length(0), messagePayload(messageData)
{
}

Bytestream::Bytestream() : bitoffset(0), offset(0), length(0) {}

int Bytestream::readInt()
{
    bitoffset = 0;
    int result = messagePayload[offset] << 24 |
                 messagePayload[offset + 1] << 16 |
                 messagePayload[offset + 2] << 8 | messagePayload[offset + 3];
    offset += 4;

    logger.debug(std::to_string(result));
    return result;
}

int Bytestream::readInt8()
{
    bitoffset = 0;
    logger.debug(std::to_string(messagePayload[offset++]));
    return messagePayload[offset];
}

int Bytestream::readInt16()
{
    bitoffset = 0;
    int result = (messagePayload[offset] << 8);
    result += (messagePayload[offset + 1]);
    offset += 2;
    logger.debug(std::to_string(result));
    return result;
}

int Bytestream::readInt24()
{
    bitoffset = 0;
    int result = (messagePayload[offset] << 16);
    result += (messagePayload[offset + 1] << 8);
    result += (messagePayload[offset + 2]);
    offset += 3;
    logger.debug(std::to_string(result));
    return result;
}

int Bytestream::readIntLittleEndian()
{
    bitoffset = 0;
    int result = (messagePayload[offset]);
    result += (messagePayload[offset + 1] << 8);
    result += (messagePayload[offset + 2] << 16);
    result += (messagePayload[offset + 3] << 24);
    offset += 4;
    logger.debug(std::to_string(result));
    return result;
}

long Bytestream::readLong()
{
    bitoffset = 0;
    long high = static_cast<long>(readInt());
    long low = static_cast<long>(readInt());

    long result = (high << 32) | (low & 0xFFFFFFFFL);
    logger.debug("Low: " + std::to_string(low) + " High:" + std::to_string(high));
    return result;
}

long long Bytestream::readLongLong()
{
    bitoffset = 0;
    int high = readInt();
    int low = readInt();

    return (static_cast<long long>(high) << 32) |
           (static_cast<unsigned int>(low));
}

int Bytestream::readShort()
{
    bitoffset = 0;
    int result = (messagePayload[offset] << 8);
    result += (messagePayload[offset + 1]);
    offset += 2;

    logger.debug(std::to_string(result));
    return result;
}

std::string Bytestream::readString()
{
    bitoffset = 0;
    int strLength = readInt();

    if (strLength <= -1)
    {
        if (strLength != -1)
        {
            logger.warn("Negative String length encountered.");
        }
        return "";
    }
    else if (strLength > 900000)
    {
        logger.warn("Too long String encountered, length: " +
                                      std::to_string(strLength) +
                                      ", max: 900000");
        return "";
    }

    logger.debug("messagePayload.size(): " +
                                   std::to_string(messagePayload.size()) +
                                   ", offset: " + std::to_string(offset) +
                                   ", strLength: " + std::to_string(strLength));

    if (offset + strLength > messagePayload.size())
    {
        throw std::runtime_error(
            "Invalid string strLength or out-of-bounds access");
    }

    std::string result;
    try
    {
        result = std::string(
            reinterpret_cast<const char *>(&messagePayload[offset]), strLength);
    }
    catch (const std::exception &e)
    {
        logger.error(e.what());
    }

    offset += strLength;
    logger.debug(result);

    return result;
}

bool Bytestream::readBoolean()
{
    bitoffset = 0;
    logger.debug(std::to_string(offset++));
    return messagePayload[offset];
}

std::vector<uint8_t> Bytestream::readBytes(size_t length)
{
    if (offset + length > messagePayload.size())
    {
        throw std::out_of_range("Attempt to read beyond the payload size");
    }
    std::vector<uint8_t> buffer(messagePayload.begin() + offset,
                                messagePayload.begin() + offset + length);
    offset += length;

    return buffer;
}

std::string Bytestream::readCompressedString()
{
    int32_t dataLength = readInt();

    if (dataLength <= 0 || dataLength > messagePayload.size() - offset)
    {
        logger.warn("Data length is 0 or too high.");
        return "";
    }

    readIntLittleEndian();
    auto compressedData = readBytes(dataLength - 4);

    uLongf decompressedSize = dataLength * 2;
    std::vector<uint8_t> decompressedData;
    try
    {
        decompressedData.resize(decompressedSize);
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Failed to allocate decompressed data buffer");
    }

    while (true)
    {
        int status = uncompress(decompressedData.data(), &decompressedSize,
                                compressedData.data(), compressedData.size());
        if (status == Z_BUF_ERROR)
        {
            decompressedSize *= 2;
            decompressedData.resize(decompressedSize);
        }
        else if (status != Z_OK)
        {
            throw std::runtime_error("Decompression failed");
        }
        else
        {
            break;
        }
    }
    return std::string(decompressedData.begin(),
                       decompressedData.begin() + decompressedSize);
}

void Bytestream::writeInt(int value)
{
    messagePayload.push_back((value >> 24) & 0xFF);
    messagePayload.push_back((value >> 16) & 0xFF);
    messagePayload.push_back((value >> 8) & 0xFF);
    messagePayload.push_back(value & 0xFF);
    offset += 4;
}

void Bytestream::writeInt8(int value)
{
    if (offset + 1 > messagePayload.size())
    {
        messagePayload.resize(offset + 1);
    }
    messagePayload[offset] = value & 0xFF;
    offset += 1;
}

void Bytestream::writeInt16(int value)
{
    if (offset + 2 > messagePayload.size())
    {
        messagePayload.resize(offset + 2);
    }
    messagePayload[offset] = (value >> 8) & 0xFF;
    messagePayload[offset + 1] = value & 0xFF;
    offset += 2;
}

void Bytestream::writeInt24(int value)
{
    if (offset + 3 > messagePayload.size())
    {
        messagePayload.resize(offset + 3);
    }
    messagePayload[offset] = (value >> 16) & 0xFF;
    messagePayload[offset + 1] = (value >> 8) & 0xFF;
    messagePayload[offset + 2] = value & 0xFF;
    offset += 3;
}

void Bytestream::writeIntLittleEndian(int value)
{
    messagePayload.push_back(value & 0xFF);
    messagePayload.push_back((value >> 8) & 0xFF);
    messagePayload.push_back((value >> 16) & 0xFF);
    messagePayload.push_back((value >> 24) & 0xFF);
    offset += 4;
}

void Bytestream::writeLong(int high, int low)
{
    writeInt(high);
    writeInt(low);
}

void Bytestream::writeShort(int value)
{
    messagePayload.push_back((value >> 8) & 0xFF);
    messagePayload.push_back(value & 0xFF);
    offset += 2;
}

void Bytestream::writeString(const std::string &value)
{
    writeInt(value.length());
    messagePayload.insert(messagePayload.end(), value.begin(), value.end());
    offset += value.size();
}

void Bytestream::writeBoolean(bool value)
{
    if (bitoffset == 0)
    {
        messagePayload.push_back(0); // Add a new byte if the bit offset is at
                                     // the start of a new byte
    }

    int lastIndex = messagePayload.size() - 1;

    if (value)
    {
        messagePayload[lastIndex] |= (1 << bitoffset);
    }

    bitoffset = (bitoffset + 1) & 7;
}

void Bytestream::writePacketHeader(int packetID)
{
    int size = messagePayload.size();
    messagePayload.insert(messagePayload.begin(),
                          {static_cast<unsigned char>((packetID >> 8) & 0xFF),
                           static_cast<unsigned char>(packetID & 0xFF),
                           static_cast<unsigned char>((size >> 16) & 0xFF),
                           static_cast<unsigned char>((size >> 8) & 0xFF),
                           static_cast<unsigned char>(size & 0xFF)});
    offset = messagePayload.size();
}
