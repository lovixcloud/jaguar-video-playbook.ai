#pragma once

#include "capture/Synchronization/AudioVideoSynchronizer.h"
#include "core/Project/ProjectModel.h"
#include <string>

namespace jaguar {

class MediaDecoder {
public:
    MediaDecoder() = default;
    ~MediaDecoder() = default;

    bool Open(const std::string& filePath, MediaAsset& outAsset);
    bool SeekToTime(double timeSeconds);
    bool ReadNextFrame(MediaFrame& outFrame);
    void Close();

private:
    std::string m_filePath;
    double m_currentTimeSeconds{ 0.0 };
    double m_durationSeconds{ 10.0 };
    int m_width{ 1920 };
    int m_height{ 1080 };
    double m_fps{ 60.0 };
    bool m_isOpen{ false };
};

} // namespace jaguar
