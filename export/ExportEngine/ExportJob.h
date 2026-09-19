#pragma once

#include "core/Project/ProjectModel.h"
#include "media/Encoder/MediaEncoder.h"
#include <string>
#include <atomic>
#include <functional>

namespace jaguar {

enum class ExportState {
    Waiting,
    Rendering,
    Paused,
    Completed,
    Failed,
    Cancelled
};

struct ExportProgress {
    int currentFrame{ 0 };
    int totalFrames{ 0 };
    double percentage{ 0.0 };
    double encodingFps{ 0.0 };
    double elapsedTimeSeconds{ 0.0 };
    double estimatedRemainingSeconds{ 0.0 };
    int64_t currentSizeBytes{ 0 };
};

class ExportJob {
public:
    using ProgressCallback = std::function<void(const ExportProgress&)>;

    ExportJob(const std::string& id, const Project& project, const EncoderConfig& config);

    void Start(ProgressCallback callback = nullptr);
    void Pause();
    void Resume();
    void Cancel();

    std::string GetId() const { return m_id; }
    ExportState GetState() const { return m_state; }
    ExportProgress GetProgress() const { return m_progress; }
    EncoderConfig GetConfig() const { return m_config; }
    std::string GetErrorMessage() const { return m_errorMessage; }

private:
    std::string m_id;
    Project m_project;
    EncoderConfig m_config;
    std::atomic<ExportState> m_state{ ExportState::Waiting };
    ExportProgress m_progress;
    std::string m_errorMessage;
};

} // namespace jaguar
