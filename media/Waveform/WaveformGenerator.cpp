#include "WaveformGenerator.h"
#include "media/Cache/MediaCache.h"
#include <cmath>

namespace jaguar {

void WaveformGenerator::GenerateWaveformAsync(const std::string& mediaId, const std::string& filePath, WaveformCallback callback) {
    (void)filePath;
    if (MediaCache::Instance().HasWaveform(mediaId)) {
        if (callback) {
            callback(MediaCache::Instance().GetWaveform(mediaId));
        }
        return;
    }

    TaskScheduler::Instance().Enqueue([mediaId, callback]() {
        std::vector<float> peaks(200);
        for (size_t i = 0; i < peaks.size(); ++i) {
            peaks[i] = std::abs(std::sin(i * 0.1f) * 0.8f + 0.1f);
        }

        MediaCache::Instance().StoreWaveform(mediaId, peaks);

        if (callback) {
            callback(peaks);
        }
    });
}

} // namespace jaguar
