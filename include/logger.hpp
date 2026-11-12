#pragma once

#include <string>
#include <fstream>

class Logger{
private:
    std::ofstream file_;
    void write(const std::string& level, const std::string& message);
    
public:
    Logger(const std::string& filename);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
};