#include "StatusSystem.h"

namespace jaguar {

StatusSystem& StatusSystem::Instance() {
    static StatusSystem instance;
    return instance;
}

void StatusSystem::NotifyInfo(const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_notifications.push_back({ message, false, 0.0 });
}

void StatusSystem::NotifyError(const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_notifications.push_back({ message, true, 0.0 });
}

std::string StatusSystem::GetLastNotification() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_notifications.empty()) {
        return m_notifications.back().message;
    }
    return "Ready";
}

} // namespace jaguar
