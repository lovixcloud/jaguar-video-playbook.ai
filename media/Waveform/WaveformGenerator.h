#pragma once

#include "core/Tasks/TaskScheduler.h"
#include <string>
#include <vector>
#include <functional>

namespace jaguar {

class WaveformGenerator {
public:
    using WaveformCallback = std::function<void(const std::vector<float>&)>;

    static void GenerateWaveformAsync(const std::string& mediaId, const std::string& filePath, WaveformCallback callback);
};

} // namespace jaguar
