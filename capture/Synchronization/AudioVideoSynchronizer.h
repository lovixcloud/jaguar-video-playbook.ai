#pragma once

#include <cstdint>
#include <mutex>
#include <vector>

namespace jaguar {

struct MediaFrame {
    enum class Type { Video, Audio };
    Type type{ Type::Video };
    int64_t timestampMs{ 0 };
    int width{ 0 };
    int height{ 0 };
    std::vector<uint8_t> pixelData;
    int sampleRate{ 48000 };
    int channels{ 2 };
    std::vector<float> audioSamples;
};

class AudioVideoSynchronizer {
public:
    AudioVideoSynchronizer() = default;

    void StartSession();
    void Reset();

    int64_t NormalizeTimestamp(int64_t rawTimestampMs);
    bool AlignFrame(MediaFrame& frame);

private:
    std::mutex m_mutex;
    int64_t m_baseTimestampMs{ -1 };
};

} // namespace jaguar
