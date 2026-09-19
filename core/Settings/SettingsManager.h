#pragma once

#include <string>
#include <mutex>

namespace jaguar {

struct Settings {
    std::string theme{ "Dark" };
    std::string language{ "English" };
    int autosaveIntervalSeconds{ 60 };

    int defaultResolutionWidth{ 1920 };
    int defaultResolutionHeight{ 1080 };
    int defaultFps{ 60 };
    std::string defaultOutputPath{ "C:\\JaguarRecordings" };
    int countdownSeconds{ 3 };
    std::string startStopHotkey{ "F9" };
    std::string pauseResumeHotkey{ "F10" };

    std::string defaultMicrophone{ "Default Microphone" };
    std::string defaultAudioOutput{ "Default Speaker" };
    int sampleRate{ 48000 };

    bool enableGpuAcceleration{ true };
    std::string previewQuality{ "High" };
    std::string cacheDirectory{ "C:\\JaguarCache" };
    int workerThreads{ 4 };

    std::string defaultCodec{ "H264" };
    int defaultVideoBitrate{ 10000000 };
    int defaultAudioBitrate{ 192000 };
};

class SettingsManager {
public:
    static SettingsManager& Instance();

    void Load(const std::string& settingsFilePath);
    void Save(const std::string& settingsFilePath);

    Settings GetSettings() const;
    void UpdateSettings(const Settings& settings);

private:
    SettingsManager() = default;

    mutable std::mutex m_mutex;
    Settings m_settings;
    std::string m_filePath;
};

} // namespace jaguar
