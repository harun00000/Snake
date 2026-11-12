#include "logger.hpp"

Logger::Logger(const std::string& filename) : file_(filename) {}

void Logger::info(const std::string& message){
    write("INFO", message);
}

void Logger::warning(const std::string& message){
    write("WARNING", message);
}

void Logger::error(const std::string& message){
    write("ERROR", message);
}

void Logger::write(const std::string& level, const std::string& message){
    if (file_.is_open())
    {
        file_ << "[" << level << "] " << message << "\n";
    }
}