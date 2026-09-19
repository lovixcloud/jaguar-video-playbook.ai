#pragma once

#include "render/Compositor/Compositor.h"
#include <atomic>
#include <functional>

namespace jaguar {

class PreviewEngine {
public:
    using FrameRenderCallback = std::function<void(const MediaFrame&)>;

    PreviewEngine();
    ~PreviewEngine();

    void Play();
    void Pause();
    void Stop();
    void Seek(double timeSeconds);
    void SetPlaybackSpeed(double speed) { m_playbackSpeed = speed; }

    bool IsPlaying() const { return m_isPlaying; }
    double GetCurrentTime() const { return m_currentTimeSeconds; }

    void StepFrame(bool forward, double fps = 60.0);
    void RenderCurrentFrame(const Project& project, FrameRenderCallback callback);

private:
    Compositor m_compositor;
    std::atomic<bool> m_isPlaying{ false };
    double m_currentTimeSeconds{ 0.0 };
    double m_playbackSpeed{ 1.0 };
};

} // namespace jaguar
