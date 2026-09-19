#include "SettingsManager.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace jaguar {

SettingsManager& SettingsManager::Instance() {
    static SettingsManager instance;
    return instance;
}

Settings SettingsManager::GetSettings() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_settings;
}

void SettingsManager::UpdateSettings(const Settings& settings) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_settings = settings;
    if (!m_filePath.empty()) {
        Save(m_filePath);
    }
}

void SettingsManager::Load(const std::string& settingsFilePath) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_filePath = settingsFilePath;
    std::ifstream file(settingsFilePath);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        auto pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            if (key == "theme") m_settings.theme = value;
            else if (key == "defaultFps") m_settings.defaultFps = std::stoi(value);
            else if (key == "defaultWidth") m_settings.defaultResolutionWidth = std::stoi(value);
            else if (key == "defaultHeight") m_settings.defaultResolutionHeight = std::stoi(value);
            else if (key == "defaultOutputPath") m_settings.defaultOutputPath = value;
        }
    }
}

void SettingsManager::Save(const std::string& settingsFilePath) {
    m_filePath = settingsFilePath;
    std::filesystem::path p(settingsFilePath);
    if (p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }

    std::ofstream file(settingsFilePath);
    if (!file.is_open()) return;

    file << "theme=" << m_settings.theme << "\n";
    file << "defaultFps=" << m_settings.defaultFps << "\n";
    file << "defaultWidth=" << m_settings.defaultResolutionWidth << "\n";
    file << "defaultHeight=" << m_settings.defaultResolutionHeight << "\n";
    file << "defaultOutputPath=" << m_settings.defaultOutputPath << "\n";
}

} // namespace jaguar
