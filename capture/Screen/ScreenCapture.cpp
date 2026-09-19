#include "ScreenCapture.h"
#include "core/Logging/Logger.h"
#include <chrono>
#include <windows.h>

namespace jaguar {

ScreenCapture::ScreenCapture() = default;

ScreenCapture::~ScreenCapture() {
    StopCapture();
}

std::vector<DisplayInfo> ScreenCapture::EnumerateDisplays() {
    std::vector<DisplayInfo> displays;
    DisplayInfo primary;
    primary.id = 0;
    primary.name = "Primary Monitor";
    primary.width = GetSystemMetrics(SM_CXSCREEN);
    primary.height = GetSystemMetrics(SM_CYSCREEN);
    primary.x = 0;
    primary.y = 0;
    primary.isPrimary = true;
    displays.push_back(primary);
    return displays;
}

bool ScreenCapture::StartCapture(int displayId, const CaptureRegion& region, int fps, FrameCallback callback) {
    if (m_isCapturing) return false;

    m_isCapturing = true;
    m_captureThread = std::thread(&ScreenCapture::CaptureThreadFunc, this, displayId, region, fps, callback);
    LOG_INFO("ScreenCapture", "Started screen capture");
    return true;
}

void ScreenCapture::StopCapture() {
    m_isCapturing = false;
    if (m_captureThread.joinable()) {
        m_captureThread.join();
    }
    LOG_INFO("ScreenCapture", "Stopped screen capture");
}

void ScreenCapture::CaptureThreadFunc(int displayId, CaptureRegion region, int fps, FrameCallback callback) {
    (void)displayId;
    int frameIntervalMs = 1000 / (fps > 0 ? fps : 60);

    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, region.width, region.height);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    BITMAPINFOHEADER bi = {};
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = region.width;
    bi.biHeight = -region.height;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;

    auto startTime = std::chrono::steady_clock::now();

    while (m_isCapturing) {
        auto frameStart = std::chrono::steady_clock::now();

        BitBlt(hdcMem, 0, 0, region.width, region.height, hdcScreen, region.x, region.y, SRCCOPY);

        MediaFrame frame;
        frame.type = MediaFrame::Type::Video;
        frame.width = region.width;
        frame.height = region.height;
        frame.pixelData.resize(region.width * region.height * 4);

        GetDIBits(hdcMem, hBitmap, 0, region.height, frame.pixelData.data(), (BITMAPINFO*)&bi, DIB_RGB_COLORS);

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

    SelectObject(hdcMem, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
}

} // namespace jaguar
