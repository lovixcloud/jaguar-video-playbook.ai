#include "MediaCache.h"
#include <filesystem>

namespace jaguar {

MediaCache& MediaCache::Instance() {
    static MediaCache instance;
    return instance;
}

void MediaCache::SetCacheDir(const std::string& dirPath) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cacheDir = dirPath;
    std::filesystem::create_directories(m_cacheDir);
}

bool MediaCache::HasWaveform(const std::string& mediaId) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_waveformPeaks.find(mediaId) != m_waveformPeaks.end();
}

void MediaCache::StoreWaveform(const std::string& mediaId, const std::vector<float>& peaks) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_waveformPeaks[mediaId] = peaks;
}

std::vector<float> MediaCache::GetWaveform(const std::string& mediaId) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_waveformPeaks.find(mediaId);
    if (it != m_waveformPeaks.end()) {
        return it->second;
    }
    return {};
}

} // namespace jaguar
