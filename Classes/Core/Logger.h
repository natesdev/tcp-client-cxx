#pragma once

#include <string>

class Logger
{
public:
    Logger(const std::string &className);

    void Info(const std::string &methodName, const std::string &message);
    void Error(const std::string &methodName, const std::string &message);
    void Warn(const std::string &methodName, const std::string &message);
    void Debug(const std::string &methodName, const std::string &message);

private:
    std::string className;
};