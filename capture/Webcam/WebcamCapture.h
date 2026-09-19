#pragma once

#include "capture/Synchronization/AudioVideoSynchronizer.h"
#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <functional>

namespace jaguar {

struct WebcamDeviceInfo {
    int id{ 0 };
    std::string name;
};

class WebcamCapture {
public:
    using FrameCallback = std::function<void(const MediaFrame&)>;

    WebcamCapture();
    ~WebcamCapture();

    static std::vector<WebcamDeviceInfo> EnumerateWebcams();

    bool StartCapture(int deviceId, int width, int height, int fps, FrameCallback callback);
    void StopCapture();
    bool IsCapturing() const { return m_isCapturing; }

private:
    void CaptureThreadFunc(int deviceId, int width, int height, int fps, FrameCallback callback);

    std::atomic<bool> m_isCapturing{ false };
    std::thread m_captureThread;
};

} // namespace jaguar
