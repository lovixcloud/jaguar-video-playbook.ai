#include "SystemAudioCapture.h"
#include "core/Logging/Logger.h"
#include <chrono>

namespace jaguar {

SystemAudioCapture::SystemAudioCapture() = default;

SystemAudioCapture::~SystemAudioCapture() {
    StopCapture();
}

std::vector<AudioDeviceInfo> SystemAudioCapture::EnumerateOutputDevices() {
    std::vector<AudioDeviceInfo> devices;
    AudioDeviceInfo out;
    out.id = 0;
    out.name = "System Audio Loopback";
    devices.push_back(out);
    return devices;
}

bool SystemAudioCapture::StartCapture(int deviceId, int sampleRate, int channels, FrameCallback callback) {
    if (m_isCapturing) return false;

    m_isCapturing = true;
    m_captureThread = std::thread(&SystemAudioCapture::CaptureThreadFunc, this, deviceId, sampleRate, channels, callback);
    LOG_INFO("SystemAudioCapture", "Started system audio loopback capture");
    return true;
}

void SystemAudioCapture::StopCapture() {
    m_isCapturing = false;
    if (m_captureThread.joinable()) {
        m_captureThread.join();
    }
    LOG_INFO("SystemAudioCapture", "Stopped system audio capture");
}

void SystemAudioCapture::CaptureThreadFunc(int deviceId, int sampleRate, int channels, FrameCallback callback) {
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
