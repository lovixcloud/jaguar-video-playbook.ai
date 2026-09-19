#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>

namespace jaguar {

Logger& Logger::Instance() {
    static Logger instance;
    return instance;
}

Logger::~Logger() {
    Shutdown();
}

void Logger::Initialize(const std::string& logFilePath) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_logPath = logFilePath;

    std::filesystem::path p(logFilePath);
    if (p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }

    m_logFile.open(m_logPath, std::ios::out | std::ios::app);
}

void Logger::Shutdown() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

void Logger::Log(LogLevel level, const std::string& subsystem, const std::string& message) {
    if (level < m_minLevel) return;

    std::lock_guard<std::mutex> lock(m_mutex);
    std::string timeStr = GetCurrentTimestamp();
    std::string levelStr = LevelToString(level);

    std::ostringstream ss;
    ss << "[" << timeStr << "] [" << levelStr << "] [" << subsystem << "] " << message;
    std::string formatted = ss.str();

    if (m_logFile.is_open()) {
        m_logFile << formatted << std::endl;
        m_logFile.flush();
    }

    m_recentLogs.push_back(formatted);
    if (m_recentLogs.size() > 500) {
        m_recentLogs.erase(m_recentLogs.begin());
    }
}

std::vector<std::string> Logger::GetRecentLogs(size_t maxCount) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_recentLogs.size() <= maxCount) {
        return m_recentLogs;
    }
    return std::vector<std::string>(m_recentLogs.end() - maxCount, m_recentLogs.end());
}

std::string Logger::LevelToString(LogLevel level) const {
    switch (level) {
    case LogLevel::Trace:    return "TRACE";
    case LogLevel::Debug:    return "DEBUG";
    case LogLevel::Info:     return "INFO ";
    case LogLevel::Warning:  return "WARN ";
    case LogLevel::Error:    return "ERROR";
    case LogLevel::Critical: return "CRIT ";
    default:                 return "INFO ";
    }
}

std::string Logger::GetCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S")
       << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

} // namespace jaguar
