#include "AudioVideoSynchronizer.h"
#include <chrono>

namespace jaguar {

void AudioVideoSynchronizer::StartSession() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_baseTimestampMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
}

void AudioVideoSynchronizer::Reset() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_baseTimestampMs = -1;
}

int64_t AudioVideoSynchronizer::NormalizeTimestamp(int64_t rawTimestampMs) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_baseTimestampMs < 0) {
        m_baseTimestampMs = rawTimestampMs;
    }
    return rawTimestampMs - m_baseTimestampMs;
}

bool AudioVideoSynchronizer::AlignFrame(MediaFrame& frame) {
    frame.timestampMs = NormalizeTimestamp(frame.timestampMs);
    return true;
}

} // namespace jaguar
