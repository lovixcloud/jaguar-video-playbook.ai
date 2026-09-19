#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <vector>

namespace jaguar {

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

class Logger {
public:
    static Logger& Instance();

    void Initialize(const std::string& logFilePath);
    void Shutdown();

    void Log(LogLevel level, const std::string& subsystem, const std::string& message);

    void SetMinLevel(LogLevel level) { m_minLevel = level; }
    LogLevel GetMinLevel() const { return m_minLevel; }

    std::vector<std::string> GetRecentLogs(size_t maxCount = 100) const;

private:
    Logger() = default;
    ~Logger();

    std::string LevelToString(LogLevel level) const;
    std::string GetCurrentTimestamp() const;

    mutable std::mutex m_mutex;
    std::ofstream m_logFile;
    std::string m_logPath;
    LogLevel m_minLevel{ LogLevel::Info };
    std::vector<std::string> m_recentLogs;
};

#define LOG_TRACE(subsystem, msg) jaguar::Logger::Instance().Log(jaguar::LogLevel::Trace, subsystem, msg)
#define LOG_DEBUG(subsystem, msg) jaguar::Logger::Instance().Log(jaguar::LogLevel::Debug, subsystem, msg)
#define LOG_INFO(subsystem, msg)  jaguar::Logger::Instance().Log(jaguar::LogLevel::Info, subsystem, msg)
#define LOG_WARN(subsystem, msg)  jaguar::Logger::Instance().Log(jaguar::LogLevel::Warning, subsystem, msg)
#define LOG_ERROR(subsystem, msg) jaguar::Logger::Instance().Log(jaguar::LogLevel::Error, subsystem, msg)
#define LOG_CRIT(subsystem, msg)  jaguar::Logger::Instance().Log(jaguar::LogLevel::Critical, subsystem, msg)

} // namespace jaguar
