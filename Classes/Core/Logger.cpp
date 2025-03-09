#include <iostream>
#include <source_location>
#include <regex>

#include "Logger.h"
#include "../../Main.h"

#define COLOR_INFO "\033[34m"
#define COLOR_ERROR "\033[31m"
#define COLOR_WARN "\033[33m"
#define COLOR_DEBUG "\033[32m"
#define COLOR_RESET "\033[0m"
#define COLOR_WHITE "\033[37m"

Logger::Logger(const std::string &className)
    : className(className) {}

std::string extractFunctionName(const std::string &fullName)
{
    static const std::regex functionPattern(R"((?:.*::)?([^:\s]+)\s*\(.*\))");
    std::smatch match;
    return std::regex_search(fullName, match, functionPattern) ? match[1].str() : fullName;
}

void Logger::info(const std::string &message, const std::source_location &loc)
{
    std::cout << COLOR_INFO << "[" << className << "::" << extractFunctionName(loc.function_name()) << " INFO] " << COLOR_RESET
              << "-> " << COLOR_WHITE << message << COLOR_RESET << std::endl;
}

void Logger::error(const std::string &message, const std::source_location &loc)
{
    if (debugLogging)
        std::cout << COLOR_ERROR << "[" << loc.file_name() << ":" << loc.line() << ":" << loc.column() << " ERROR] " << COLOR_RESET
                  << "-> " << COLOR_WHITE << message << COLOR_RESET << std::endl;
    else
        std::cout << COLOR_ERROR << "[" << className << "::" << extractFunctionName(loc.function_name()) << " ERROR] " << COLOR_RESET
                  << "-> " << COLOR_WHITE << message << COLOR_RESET << std::endl;
}

void Logger::warn(const std::string &message, const std::source_location &loc)
{
    if (debugLogging)
        std::cout << COLOR_WARN << "[" << loc.file_name() << ":" << loc.line() << ":" << loc.column() << " WARN] " << COLOR_RESET
                  << "-> " << COLOR_WHITE << message << COLOR_RESET << std::endl;
    else
        std::cout << COLOR_WARN << "[" << className << "::" << extractFunctionName(loc.function_name()) << " WARN] " << COLOR_RESET
                  << "-> " << COLOR_WHITE << message << COLOR_RESET << std::endl;
}

void Logger::debug(const std::string &message, const std::source_location &loc)
{
    if (debugLogging)
        std::cout << COLOR_DEBUG << "[" << loc.file_name() << ":" << loc.line() << ":" << loc.column() << " DEBUG] " << COLOR_RESET
                  << "-> " << COLOR_WHITE << message << COLOR_RESET << std::endl;
}
