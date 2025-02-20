#include <iostream>

#include "Logger.h"

#define COLOR_INFO "\033[34m"
#define COLOR_ERROR "\033[31m"
#define COLOR_WARN "\033[33m"
#define COLOR_DEBUG "\033[32m"
#define COLOR_RESET "\033[0m"
#define COLOR_WHITE "\033[37m"

Logger::Logger(const std::string &className, bool debugLogging)
    : className(className), debugLogging(debugLogging) {}

void Logger::Info(const std::string &methodName, const std::string &message)
{
    std::cout << COLOR_INFO << "[" << className << "::" << methodName << " " << "Info" << "]" << COLOR_RESET
              << " -> " << COLOR_WHITE << message << COLOR_RESET << std::endl;
}

void Logger::Error(const std::string &methodName, const std::string &message)
{
    std::cout << COLOR_ERROR << "[" << className << "::" << methodName << " " << "ERR" << "]" << COLOR_RESET
              << " -> " << COLOR_WHITE << message << COLOR_RESET << std::endl;
}

void Logger::Warn(const std::string &methodName, const std::string &message)
{
    std::cout << COLOR_WARN << "[" << className << "::" << methodName << " " << "Warn" << "]" << COLOR_RESET
              << " -> " << COLOR_WHITE << message << COLOR_RESET << std::endl;
}

void Logger::Debug(const std::string &methodName, const std::string &message)
{
    if (debugLogging)
    std::cout << COLOR_DEBUG << "[" << className << "::" << methodName << " " << "Debug" << "]" << COLOR_RESET
              << " -> " << COLOR_WHITE << message << COLOR_RESET << std::endl;
}