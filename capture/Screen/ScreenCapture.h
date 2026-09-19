#pragma once

#include "capture/Synchronization/AudioVideoSynchronizer.h"
#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <functional>

namespace jaguar {

struct DisplayInfo {
    int id{ 0 };
    std::string name;
    int width{ 1920 };
    int height{ 1080 };
    int x{ 0 };
    int y{ 0 };
    bool isPrimary{ true };
};

struct CaptureRegion {
    int x{ 0 };
    int y{ 0 };
    int width{ 1920 };
    int height{ 1080 };
};

class ScreenCapture {
public:
    using FrameCallback = std::function<void(const MediaFrame&)>;

    ScreenCapture();
    ~ScreenCapture();

    static std::vector<DisplayInfo> EnumerateDisplays();

    bool StartCapture(int displayId, const CaptureRegion& region, int fps, FrameCallback callback);
    void StopCapture();
    bool IsCapturing() const { return m_isCapturing; }

private:
    void CaptureThreadFunc(int displayId, CaptureRegion region, int fps, FrameCallback callback);

    std::atomic<bool> m_isCapturing{ false };
    std::thread m_captureThread;
};

} // namespace jaguar
