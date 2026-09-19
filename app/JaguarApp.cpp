#include "JaguarApp.h"
#include "core/Logging/Logger.h"
#include "core/Settings/SettingsManager.h"
#include "core/Tasks/TaskScheduler.h"

namespace jaguar {

JaguarApp::JaguarApp() = default;
JaguarApp::~JaguarApp() {
    Logger::Instance().Shutdown();
    TaskScheduler::Instance().Shutdown();
}

bool JaguarApp::Initialize(HINSTANCE hInstance, int nCmdShow) {
    Logger::Instance().Initialize("jaguar.log");
    LOG_INFO("App", "Initializing Jaguar Studio...");

    TaskScheduler::Instance().Initialize(4);
    SettingsManager::Instance().Load("jaguar_settings.ini");

    m_window = std::make_unique<JaguarStudioWindow>();
    if (!m_window->Create(hInstance, nCmdShow)) {
        LOG_CRIT("App", "Failed to create Jaguar Studio window.");
        return false;
    }

    return true;
}

int JaguarApp::Run() {
    if (m_window) {
        return m_window->RunMessageLoop();
    }
    return 0;
}

} // namespace jaguar
