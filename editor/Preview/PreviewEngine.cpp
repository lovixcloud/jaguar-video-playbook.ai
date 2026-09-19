#include "PreviewEngine.h"

namespace jaguar {

PreviewEngine::PreviewEngine() = default;
PreviewEngine::~PreviewEngine() = default;

void PreviewEngine::Play() {
    m_isPlaying = true;
}

void PreviewEngine::Pause() {
    m_isPlaying = false;
}

void PreviewEngine::Stop() {
    m_isPlaying = false;
    m_currentTimeSeconds = 0.0;
}

void PreviewEngine::Seek(double timeSeconds) {
    m_currentTimeSeconds = timeSeconds < 0.0 ? 0.0 : timeSeconds;
}

void PreviewEngine::StepFrame(bool forward, double fps) {
    double step = 1.0 / (fps > 0 ? fps : 60.0);
    if (forward) {
        m_currentTimeSeconds += step;
    } else {
        m_currentTimeSeconds = std::max(0.0, m_currentTimeSeconds - step);
    }
}

void PreviewEngine::RenderCurrentFrame(const Project& project, FrameRenderCallback callback) {
    MediaFrame frame = m_compositor.CompositeFrame(project, m_currentTimeSeconds);
    if (callback) {
        callback(frame);
    }
}

} // namespace jaguar
