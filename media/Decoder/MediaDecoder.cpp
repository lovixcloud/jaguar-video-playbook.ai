#include "MediaDecoder.h"
#include "core/Logging/Logger.h"
#include <filesystem>

namespace jaguar {

bool MediaDecoder::Open(const std::string& filePath, MediaAsset& outAsset) {
    if (!std::filesystem::exists(filePath)) {
        LOG_WARN("MediaDecoder", "File not found: " + filePath);
        return false;
    }

    m_filePath = filePath;
    m_durationSeconds = 10.0;
    m_width = 1920;
    m_height = 1080;
    m_fps = 60.0;
    m_currentTimeSeconds = 0.0;
    m_isOpen = true;

    outAsset.filePath = filePath;
    outAsset.fileName = std::filesystem::path(filePath).filename().string();
    outAsset.durationSeconds = m_durationSeconds;
    outAsset.width = m_width;
    outAsset.height = m_height;
    outAsset.fps = m_fps;
    outAsset.codec = "H264";
    outAsset.fileSizeBytes = std::filesystem::file_size(filePath);

    LOG_INFO("MediaDecoder", "Opened media asset: " + filePath);
    return true;
}

bool MediaDecoder::SeekToTime(double timeSeconds) {
    if (!m_isOpen) return false;
    m_currentTimeSeconds = timeSeconds;
    return true;
}

bool MediaDecoder::ReadNextFrame(MediaFrame& outFrame) {
    if (!m_isOpen || m_currentTimeSeconds >= m_durationSeconds) return false;

    outFrame.type = MediaFrame::Type::Video;
    outFrame.width = m_width;
    outFrame.height = m_height;
    outFrame.timestampMs = static_cast<int64_t>(m_currentTimeSeconds * 1000.0);
    outFrame.pixelData.resize(m_width * m_height * 4, 100);

    m_currentTimeSeconds += 1.0 / m_fps;
    return true;
}

void MediaDecoder::Close() {
    m_isOpen = false;
}

} // namespace jaguar
