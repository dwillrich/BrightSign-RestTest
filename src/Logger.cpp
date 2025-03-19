#include "Logger.h"

std::mutex Logger::logMutex;

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::logInfo(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << "[INFO] " << message << std::endl;
}

void Logger::logError(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::cerr << "[ERROR] " << message << std::endl;
}
