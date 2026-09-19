#include "ExportJob.h"
#include "render/Compositor/Compositor.h"
#include "core/Logging/Logger.h"
#include <chrono>
#include <thread>

namespace jaguar {

ExportJob::ExportJob(const std::string& id, const Project& project, const EncoderConfig& config)
    : m_id(id), m_project(project), m_config(config) {}

void ExportJob::Start(ProgressCallback callback) {
    if (m_state == ExportState::Rendering) return;

    m_state = ExportState::Rendering;
    LOG_INFO("ExportJob", "Starting export job: " + m_id);

    MediaEncoder encoder;
    if (!encoder.Initialize(m_config)) {
        m_state = ExportState::Failed;
        m_errorMessage = "Failed to initialize encoder.";
        return;
    }

    Compositor compositor;
    double fps = m_config.fps > 0 ? m_config.fps : 60.0;
    double totalDuration = m_project.totalDurationSeconds > 0 ? m_project.totalDurationSeconds : 10.0;
    m_progress.totalFrames = static_cast<int>(totalDuration * fps);

    auto startTime = std::chrono::steady_clock::now();

    for (int frameIdx = 0; frameIdx < m_progress.totalFrames; ++frameIdx) {
        if (m_state == ExportState::Cancelled) {
            encoder.Finalize();
            LOG_INFO("ExportJob", "Export job cancelled: " + m_id);
            return;
        }

        while (m_state == ExportState::Paused) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        double timeSec = frameIdx / fps;
        MediaFrame frame = compositor.CompositeFrame(m_project, timeSec);
        encoder.EncodeFrame(frame);

        m_progress.currentFrame = frameIdx + 1;
        m_progress.percentage = (static_cast<double>(m_progress.currentFrame) / m_progress.totalFrames) * 100.0;

        auto now = std::chrono::steady_clock::now();
        m_progress.elapsedTimeSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(now - startTime).count();
        if (m_progress.elapsedTimeSeconds > 0) {
            m_progress.encodingFps = m_progress.currentFrame / m_progress.elapsedTimeSeconds;
            double remainingFrames = m_progress.totalFrames - m_progress.currentFrame;
            m_progress.estimatedRemainingSeconds = remainingFrames / m_progress.encodingFps;
        }

        if (callback) {
            callback(m_progress);
        }
    }

    encoder.Finalize();
    m_state = ExportState::Completed;
    LOG_INFO("ExportJob", "Export job completed: " + m_id);
}

void ExportJob::Pause() {
    if (m_state == ExportState::Rendering) {
        m_state = ExportState::Paused;
    }
}

void ExportJob::Resume() {
    if (m_state == ExportState::Paused) {
        m_state = ExportState::Rendering;
    }
}

void ExportJob::Cancel() {
    m_state = ExportState::Cancelled;
}

} // namespace jaguar
