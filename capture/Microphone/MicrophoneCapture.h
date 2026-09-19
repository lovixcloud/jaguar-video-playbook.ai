#pragma once

#include "capture/Synchronization/AudioVideoSynchronizer.h"
#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <functional>

namespace jaguar {

struct AudioDeviceInfo {
    int id{ 0 };
    std::string name;
};

class MicrophoneCapture {
public:
    using FrameCallback = std::function<void(const MediaFrame&)>;

    MicrophoneCapture();
    ~MicrophoneCapture();

    static std::vector<AudioDeviceInfo> EnumerateMicrophones();

    bool StartCapture(int deviceId, int sampleRate, int channels, FrameCallback callback);
    void StopCapture();
    bool IsCapturing() const { return m_isCapturing; }

private:
    void CaptureThreadFunc(int deviceId, int sampleRate, int channels, FrameCallback callback);

    std::atomic<bool> m_isCapturing{ false };
    std::thread m_captureThread;
};

} // namespace jaguar
