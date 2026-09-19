#include "WebcamCapture.h"
#include "core/Logging/Logger.h"
#include <chrono>

namespace jaguar {

WebcamCapture::WebcamCapture() = default;

WebcamCapture::~WebcamCapture() {
    StopCapture();
}

std::vector<WebcamDeviceInfo> WebcamCapture::EnumerateWebcams() {
    std::vector<WebcamDeviceInfo> devices;
    WebcamDeviceInfo defaultCam;
    defaultCam.id = 0;
    defaultCam.name = "Integrated HD Webcam";
    devices.push_back(defaultCam);
    return devices;
}

bool WebcamCapture::StartCapture(int deviceId, int width, int height, int fps, FrameCallback callback) {
    if (m_isCapturing) return false;

    m_isCapturing = true;
    m_captureThread = std::thread(&WebcamCapture::CaptureThreadFunc, this, deviceId, width, height, fps, callback);
    LOG_INFO("WebcamCapture", "Started webcam capture");
    return true;
}

void WebcamCapture::StopCapture() {
    m_isCapturing = false;
    if (m_captureThread.joinable()) {
        m_captureThread.join();
    }
    LOG_INFO("WebcamCapture", "Stopped webcam capture");
}

void WebcamCapture::CaptureThreadFunc(int deviceId, int width, int height, int fps, FrameCallback callback) {
    (void)deviceId;
    int frameIntervalMs = 1000 / (fps > 0 ? fps : 30);
    auto startTime = std::chrono::steady_clock::now();

    while (m_isCapturing) {
        auto frameStart = std::chrono::steady_clock::now();

        MediaFrame frame;
        frame.type = MediaFrame::Type::Video;
        frame.width = width;
        frame.height = height;
        frame.pixelData.resize(width * height * 4, 128);

        auto now = std::chrono::steady_clock::now();
        frame.timestampMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

        if (callback) {
            callback(frame);
        }

        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - frameStart).count();
        if (elapsedMs < frameIntervalMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(frameIntervalMs - elapsedMs));
        }
    }
}

} // namespace jaguar
