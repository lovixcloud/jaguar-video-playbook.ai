#pragma once

#include <string>
#include <mutex>
#include <vector>

namespace jaguar {

struct Notification {
    std::string message;
    bool isError{ false };
    double timestampSeconds{ 0.0 };
};

class StatusSystem {
public:
    static StatusSystem& Instance();

    void NotifyInfo(const std::string& message);
    void NotifyError(const std::string& message);

    std::string GetLastNotification() const;

private:
    StatusSystem() = default;

    mutable std::mutex m_mutex;
    std::vector<Notification> m_notifications;
};

} // namespace jaguar
