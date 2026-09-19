#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

namespace jaguar {

class MediaCache {
public:
    static MediaCache& Instance();

    void SetCacheDir(const std::string& dirPath);
    std::string GetCacheDir() const { return m_cacheDir; }

    bool HasWaveform(const std::string& mediaId) const;
    void StoreWaveform(const std::string& mediaId, const std::vector<float>& peaks);
    std::vector<float> GetWaveform(const std::string& mediaId) const;

private:
    MediaCache() = default;

    mutable std::mutex m_mutex;
    std::string m_cacheDir{ "C:\\JaguarCache" };
    std::unordered_map<std::string, std::vector<float>> m_waveformPeaks;
};

} // namespace jaguar
