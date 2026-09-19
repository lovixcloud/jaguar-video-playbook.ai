#include "MicrophoneCapture.h"
#include "core/Logging/Logger.h"
#include <chrono>

namespace jaguar {

MicrophoneCapture::MicrophoneCapture() = default;

MicrophoneCapture::~MicrophoneCapture() {
    StopCapture();
}

std::vector<AudioDeviceInfo> MicrophoneCapture::EnumerateMicrophones() {
    std::vector<AudioDeviceInfo> devices;
    AudioDeviceInfo mic;
    mic.id = 0;
    mic.name = "Default Microphone";
    devices.push_back(mic);
    return devices;
}

bool MicrophoneCapture::StartCapture(int deviceId, int sampleRate, int channels, FrameCallback callback) {
    if (m_isCapturing) return false;

    m_isCapturing = true;
    m_captureThread = std::thread(&MicrophoneCapture::CaptureThreadFunc, this, deviceId, sampleRate, channels, callback);
    LOG_INFO("MicrophoneCapture", "Started microphone capture");
    return true;
}

void MicrophoneCapture::StopCapture() {
    m_isCapturing = false;
    if (m_captureThread.joinable()) {
        m_captureThread.join();
    }
    LOG_INFO("MicrophoneCapture", "Stopped microphone capture");
}

void MicrophoneCapture::CaptureThreadFunc(int deviceId, int sampleRate, int channels, FrameCallback callback) {
    (void)deviceId;
    int chunkSamples = sampleRate / 50;
    auto startTime = std::chrono::steady_clock::now();

    while (m_isCapturing) {
        auto frameStart = std::chrono::steady_clock::now();

        MediaFrame frame;
        frame.type = MediaFrame::Type::Audio;
        frame.sampleRate = sampleRate;
        frame.channels = channels;
        frame.audioSamples.resize(chunkSamples * channels, 0.0f);

        auto now = std::chrono::steady_clock::now();
        frame.timestampMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

        if (callback) {
            callback(frame);
        }

        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - frameStart).count();
        if (elapsedMs < 20) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20 - elapsedMs));
        }
    }
}

} // namespace jaguar
