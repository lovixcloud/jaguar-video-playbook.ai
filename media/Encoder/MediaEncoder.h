#pragma once

#include "capture/Synchronization/AudioVideoSynchronizer.h"
#include <string>
#include <fstream>

namespace jaguar {

struct EncoderConfig {
    std::string outputPath;
    int width{ 1920 };
    int height{ 1080 };
    int fps{ 60 };
    int videoBitrate{ 10000000 };
    int audioSampleRate{ 48000 };
    int audioChannels{ 2 };
    int audioBitrate{ 192000 };
    std::string codec{ "H264" };
    std::string container{ "MP4" };
};

class MediaEncoder {
public:
    MediaEncoder() = default;
    ~MediaEncoder() = default;

    bool Initialize(const EncoderConfig& config);
    bool EncodeFrame(const MediaFrame& frame);
    bool Finalize();

private:
    EncoderConfig m_config;
    std::ofstream m_outputFile;
    bool m_isInitialized{ false };
    int64_t m_encodedFrames{ 0 };
};

} // namespace jaguar
