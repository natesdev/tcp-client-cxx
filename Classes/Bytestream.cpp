#include <iostream>
#include <zlib.h>
#include <cstring>
#include <vector>
#include <algorithm>

#include "Bytestream.h"
//#include "ChecksumEncoder.h"
#include "Core/Logger.h"

Logger logger("Bytestream");

Bytestream::Bytestream(const std::vector<uint8_t> &messageData) : bitoffset(0), offset(0), length(0), messagePayload(messageData) {}

Bytestream::Bytestream() : bitoffset(0), offset(0), length(0) {}

int Bytestream::readInt()
{
    bitoffset = 0;
    int result = messagePayload[offset] << 24 |
                 messagePayload[offset + 1] << 16 |
                 messagePayload[offset + 2] << 8 |
                 messagePayload[offset + 3];
    offset += 4;

    logger.Debug("readInt", std::to_string(result));
    return result;
}

int Bytestream::readInt8()
{
    bitoffset = 0;
    logger.Debug("readInt8", std::to_string(messagePayload[offset++]));
    return messagePayload[offset];
}

int Bytestream::readInt16()
{
    bitoffset = 0;
    int result = (messagePayload[offset] << 8);
    result += (messagePayload[offset + 1]);
    offset += 2;
    logger.Debug("readInt16", std::to_string(result));
    return result;
}

int Bytestream::readInt24()
{
    bitoffset = 0;
    int result = (messagePayload[offset] << 16);
    result += (messagePayload[offset + 1] << 8);
    result += (messagePayload[offset + 2]);
    offset += 3;
    logger.Debug("readInt24", std::to_string(result));
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
    logger.Debug("readIntLittleEndian", std::to_string(result));
    return result;
}

LogicLong Bytestream::readLong()
{
    LogicLong logicLong;
    logicLong.decode(this);
    logger.Debug("readLong", "Low: " + std::to_string(logicLong.low) + " High:" + std::to_string(logicLong.high));
    return logicLong;
}

long long Bytestream::readLongLong()
{
    bitoffset = 0;
    int high = readInt();
    int low = readInt();

    return (static_cast<long long>(high) << 32) | (static_cast<unsigned int>(low));
}

int Bytestream::readShort()
{
    bitoffset = 0;
    int result = (messagePayload[offset] << 8);
    result += (messagePayload[offset + 1]);
    offset += 2;

    logger.Debug("readShort", std::to_string(result));
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
            logger.Warn("readString", "Negative String length encountered.");
        }
        return "";
    }
    else if (strLength > 900000)
    {
        logger.Warn("readString", "Too long String encountered, length: " + std::to_string(strLength) + ", max: 900000");
        return "";
    }

    logger.Debug("readString", "messagePayload.size(): " + std::to_string(messagePayload.size()) + ", offset: " + std::to_string(offset) + ", strLength: " + std::to_string(strLength));

    if (offset + strLength > messagePayload.size())
    {
        throw std::runtime_error("Invalid string strLength or out-of-bounds access");
    }

    std::string result;
    try
    {
        result = std::string(reinterpret_cast<const char *>(&messagePayload[offset]), strLength);
    }
    catch (const std::exception &e)
    {
        logger.Error("readString", e.what());
    }

    offset += strLength;
    logger.Debug("readString", result);

    return result;
}

/**
 * @brief Reads a variable-length integer from the bytestream.
 *
 * This function reads a variable-length integer (vint) from the message payload
 * starting at the current offset. The vint is encoded using a variable number
 * of bytes, with each byte containing 7 bits of the integer and a continuation
 * bit (the most significant bit). The continuation bit indicates whether there
 * are more bytes to read.
 *
 * The function updates the offset to point to the next byte after the last byte
 * of the vint. It also handles sign extension for negative numbers.
 *
 * @return The decoded variable-length integer.
 */
int Bytestream::readVint()
{
    int offset = this->offset;
    this->bitoffset = 0;
    int result = this->messagePayload[offset] & 0x3F;
    this->offset = offset + 1;

    if (this->messagePayload[offset] & 0x40)
    {
        if (this->messagePayload[offset] & 0x80)
        {
            result |= (this->messagePayload[offset + 1] & 0x7F) << 6;
            this->offset = offset + 2;

            if (this->messagePayload[offset + 1] & 0x80)
            {
                result |= (this->messagePayload[offset + 2] & 0x7F) << 13;
                this->offset = offset + 3;

                if (this->messagePayload[offset + 2] & 0x80)
                {
                    result |= (this->messagePayload[offset + 3] & 0x7F) << 20;
                    this->offset = offset + 4;

                    if (this->messagePayload[offset + 3] & 0x80)
                    {
                        result |= this->messagePayload[offset + 4] << 27;
                        this->offset = offset + 5;
                        return result | 0x80000000;
                    }
                    return result | 0xF8000000;
                }
                return result | 0xFFF00000;
            }
            return result | 0xFFFFE000;
        }
        return this->messagePayload[offset] | 0xFFFFFFC0;
    }
    else if (this->messagePayload[offset] & 0x80)
    {
        result |= (this->messagePayload[offset + 1] & 0x7F) << 6;
        this->offset = offset + 2;

        if (this->messagePayload[offset + 1] & 0x80)
        {
            result |= (this->messagePayload[offset + 2] & 0x7F) << 13;
            this->offset = offset + 3;

            if (this->messagePayload[offset + 2] & 0x80)
            {
                result |= (this->messagePayload[offset + 3] & 0x7F) << 20;
                this->offset = offset + 4;

                if (this->messagePayload[offset + 3] & 0x80)
                {
                    result |= this->messagePayload[offset + 4] << 27;
                    this->offset = offset + 5;
                }
            }
        }
    }
    logger.Debug("readVint", std::to_string(result));

    return result;
}

long Bytestream::readVlong()
{
    int high = readVint();
    int low = readVint();

    return (static_cast<long>(high) << 32) | (low & 0xFFFFFFFFL);
}

bool Bytestream::readBoolean()
{
    bitoffset = 0;
    logger.Debug("readInt", std::to_string(offset++));
    return messagePayload[offset];
}

LogicLong Bytestream::readDataReference()
{
    int high = readVint();
    if (high == 0)
    {
        return LogicLong(0, 0);
    }
    int low = readVint();
    return LogicLong(high, low);
}

std::vector<uint8_t> Bytestream::readBytes(size_t length)
{
    if (offset + length > messagePayload.size())
    {
        throw std::out_of_range("Attempt to read beyond the payload size");
    }
    std::vector<uint8_t> buffer(messagePayload.begin() + offset, messagePayload.begin() + offset + length);
    offset += length;

    return buffer;
}

std::string Bytestream::readCompressedString()
{
    int32_t dataLength = readInt();

    if (dataLength <= 0 || dataLength > messagePayload.size() - offset)
    {
        logger.Warn("readCompressedString", "Data length is 0 or too high.");
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
        int status = uncompress(decompressedData.data(), &decompressedSize, compressedData.data(), compressedData.size());
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
    return std::string(decompressedData.begin(), decompressedData.begin() + decompressedSize);
}

void Bytestream::writeInt8(int value)
{
    bitoffset = 0;
    std::vector<uint8_t> tempBuf(messagePayload.begin(), messagePayload.end());
    tempBuf.push_back((value >> 8) & 0xFF);
    tempBuf.push_back(value & 0xFF);
    messagePayload = std::vector<uint8_t>(tempBuf.begin(), tempBuf.end());
    offset += 2;
}

void Bytestream::writeInt16(int value)
{
    bitoffset = 0;
    std::vector<uint8_t> tempBuf(messagePayload.begin(), messagePayload.end());
    tempBuf.push_back((value >> 8) & 0xFF);
    tempBuf.push_back(value & 0xFF);
    messagePayload = std::vector<uint8_t>(tempBuf.begin(), tempBuf.end());
    offset += 2;
}

void Bytestream::writeInt24(int value)
{
    bitoffset = 0;
    std::vector<uint8_t> tempBuf(messagePayload.begin(), messagePayload.end());
    tempBuf.push_back((value >> 16) & 0xFF);
    tempBuf.push_back((value >> 8) & 0xFF);
    tempBuf.push_back(value & 0xFF);
    messagePayload = std::vector<uint8_t>(tempBuf.begin(), tempBuf.end());
    offset += 3;
}

void Bytestream::writeIntLittleEndian(int value)
{
    messagePayload.push_back(static_cast<unsigned char>((value >> 24) & 0xFF));
    messagePayload.push_back(static_cast<unsigned char>((value >> 16) & 0xFF));
    messagePayload.push_back(static_cast<unsigned char>((value >> 8) & 0xFF));
    messagePayload.push_back(static_cast<unsigned char>(value & 0xFF));
    offset += 4;
}

void Bytestream::writeInt(int value)
{
    messagePayload.push_back((value >> 24) & 0xFF);
    messagePayload.push_back((value >> 16) & 0xFF);
    messagePayload.push_back((value >> 8) & 0xFF);
    messagePayload.push_back(value & 0xFF);
    offset += 4;
}

void Bytestream::writeLong(int high, int low)
{
    writeInt(high);
    writeInt(low);
}

void Bytestream::writeLongLong(LogicLong longlong)
{
    bitoffset = 0;
    writeInt(longlong.high);
    writeInt(longlong.low);
}

void Bytestream::writeShort(int value)
{
    bitoffset = 0;
    messagePayload.push_back((value >> 8) & 0xFF);
    messagePayload.push_back(value & 0xFF);
    offset += 2;
}

void Bytestream::writeString(const std::string &value)
{
    bitoffset = 0;
    writeInt(value.length());
    messagePayload.insert(messagePayload.end(), value.begin(), value.end());
    offset += value.size();
}

void Bytestream::writeStringReference(const std::string &value)
{
    bitoffset = 0;
    messagePayload.insert(messagePayload.end(), value.begin(), value.end());
    offset += value.size();
}

void Bytestream::writeVint(int value)
{
    bitoffset = 0;
    if (value < 0)
    {
        if (value >= -63)
        {
            messagePayload.push_back((value & 0x3F) | 0x40);
            offset += 1;
        }
        else if (value >= -8191)
        {
            messagePayload.push_back((value & 0x3F) | 0xC0);
            messagePayload.push_back((value >> 6) & 0x7F);
            offset += 2;
        }
        else if (value >= -1048575)
        {
            messagePayload.push_back((value & 0x3F) | 0xC0);
            messagePayload.push_back(((value >> 6) & 0x7F) | 0x80);
            messagePayload.push_back((value >> 13) & 0x7F);
            offset += 3;
        }
        else if (value >= -134217727)
        {
            messagePayload.push_back((value & 0x3F) | 0xC0);
            messagePayload.push_back(((value >> 6) & 0x7F) | 0x80);
            messagePayload.push_back(((value >> 13) & 0x7F) | 0x80);
            messagePayload.push_back((value >> 20) & 0x7F);
            offset += 4;
        }
        else
        {
            messagePayload.push_back((value & 0x3F) | 0xC0);
            messagePayload.push_back(((value >> 6) & 0x7F) | 0x80);
            messagePayload.push_back(((value >> 13) & 0x7F) | 0x80);
            messagePayload.push_back(((value >> 20) & 0x7F) | 0x80);
            messagePayload.push_back((value >> 27) & 0xF);
            offset += 5;
        }
    }
    else
    {
        if (value <= 63)
        {
            messagePayload.push_back(value & 0x3F);
            offset += 1;
        }
        else if (value <= 8191)
        {
            messagePayload.push_back((value & 0x3F) | 0x80);
            messagePayload.push_back((value >> 6) & 0x7F);
            offset += 2;
        }
        else if (value <= 1048575)
        {
            messagePayload.push_back((value & 0x3F) | 0x80);
            messagePayload.push_back(((value >> 6) & 0x7F) | 0x80);
            messagePayload.push_back((value >> 13) & 0x7F);
            offset += 3;
        }
        else if (value <= 134217727)
        {
            messagePayload.push_back((value & 0x3F) | 0x80);
            messagePayload.push_back(((value >> 6) & 0x7F) | 0x80);
            messagePayload.push_back(((value >> 13) & 0x7F) | 0x80);
            messagePayload.push_back((value >> 20) & 0x7F);
            offset += 4;
        }
        else
        {
            messagePayload.push_back((value & 0x3F) | 0x80);
            messagePayload.push_back(((value >> 6) & 0x7F) | 0x80);
            messagePayload.push_back(((value >> 13) & 0x7F) | 0x80);
            messagePayload.push_back(((value >> 20) & 0x7F) | 0x80);
            messagePayload.push_back((value >> 27) & 0xF);
            offset += 5;
        }
    }
}

void Bytestream::writeVlong(int32_t high, int32_t low)
{
    bitoffset = 0;
    writeVint(high);
    writeVint(low);
}

void Bytestream::writeBoolean(bool value)
{
    if (bitoffset == 0)
    {
        offset++;
        messagePayload.push_back(0);
    }
    if ((value & 1) != 0)
    {
        messagePayload[offset - 1] |= 1 << (bitoffset & 31);
    }
    bitoffset = (bitoffset + 1) & 7;
}

void Bytestream::writeDataReference(int high, int low)
{
    writeVint(high);
    if (high != 0)
    {
        writeVint(low);
    }
}

void Bytestream::writeCompressedString(const std::string &data)
{
    bitoffset = 0;
    std::vector<uint8_t> compressedText(compressBound(data.size()));
    uLongf compressedSize = compressedText.size();

    if (compress(compressedText.data(), &compressedSize, reinterpret_cast<const Bytef *>(data.c_str()), data.size()) != Z_OK)
    {
        logger.Error("writeCompressedString", "Failed to decompress data");
        return;
    }

    writeInt(static_cast<int>(compressedSize) + 4);
    writeIntLittleEndian(static_cast<int>(data.size()));
    messagePayload.insert(messagePayload.end(), compressedText.begin(), compressedText.begin() + compressedSize);
}

std::vector<LogicLong> Bytestream::decodeLogicLongList()
{
    int length = readVint();
    std::vector<LogicLong> logicLongList;
    for (int i = 0; i < length; i++)
    {
        logicLongList.push_back(LogicLong(readVint(), readVint()));
    }
    return logicLongList;
}

void Bytestream::writeByte(uint8_t value)
{
    bitoffset = 0;
    messagePayload.push_back(value & 0xFF);
    offset += 1;
}

void Bytestream::writeBytes(const uint8_t value, size_t length)
{
    bitoffset = 0;
    if (value != 0)
    {
        writeInt(length);
        messagePayload.insert(messagePayload.end(), value);
        offset += length;
    }
    else
    {
        writeInt(static_cast<int>(-1));
    }
}

void Bytestream::writeHexa(const std::string &data, size_t length)
{
    bitoffset = 0;
    std::string cleanData = data;
    if (!data.empty() && data.rfind("0x", 0) == 0)
    {
        cleanData = data.substr(2);
    }
    cleanData.erase(remove_if(cleanData.begin(), cleanData.end(), [](char c)
                              { return isspace(c) || c == '-'; }),
                    cleanData.end());

    std::vector<uint8_t> bytes(cleanData.size() / 2);
    for (size_t i = 0; i < bytes.size(); ++i)
    {
        bytes[i] = std::stoi(cleanData.substr(i * 2, 2), nullptr, 16);
    }
    messagePayload.insert(messagePayload.end(), bytes.begin(), bytes.end());
    offset += length;
}

void Bytestream::encodeLogicLong(const LogicLong &logicLong)
{
    writeVint(logicLong.high);
    writeVint(logicLong.low);
}

void Bytestream::encodeLogicLongList(const std::vector<LogicLong> &logicLongList)
{
    int length = logicLongList.size();
    writeVint(length);
    for (const auto &logicLong : logicLongList)
    {
        writeVint(logicLong.high);
        writeVint(logicLong.low);
    }
}

void Bytestream::encodeIntList(const std::vector<int> &intList)
{
    int length = intList.size();
    writeVint(length);
    for (const auto &i : intList)
    {
        writeVint(i);
    }
}

void Bytestream::writePacketHeader(int packetID, int packetVersion)
{
    Bytestream tempStream;
    tempStream.writeInt16(packetID);
    tempStream.writeInt24(messagePayload.size());
    tempStream.writeInt16(packetVersion);

    messagePayload.insert(messagePayload.begin(),
                          tempStream.messagePayload.begin(),
                          tempStream.messagePayload.end());
}
