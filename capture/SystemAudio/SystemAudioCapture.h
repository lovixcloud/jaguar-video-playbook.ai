#pragma once

#include "capture/Synchronization/AudioVideoSynchronizer.h"
#include "capture/Microphone/MicrophoneCapture.h"
#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <functional>

namespace jaguar {

class SystemAudioCapture {
public:
    using FrameCallback = std::function<void(const MediaFrame&)>;

    SystemAudioCapture();
    ~SystemAudioCapture();

    static std::vector<AudioDeviceInfo> EnumerateOutputDevices();

    bool StartCapture(int deviceId, int sampleRate, int channels, FrameCallback callback);
    void StopCapture();
    bool IsCapturing() const { return m_isCapturing; }

private:
    void CaptureThreadFunc(int deviceId, int sampleRate, int channels, FrameCallback callback);

    std::atomic<bool> m_isCapturing{ false };
    std::thread m_captureThread;
};

} // namespace jaguar
