#include "ITransition.h"
#include <algorithm>
#include <cmath>

namespace jaguar {

void CrossfadeTransition::Apply(const MediaFrame& frameA, const MediaFrame& frameB, double progress, MediaFrame& outFrame) {
    outFrame = frameA;
    if (frameA.pixelData.empty() || frameB.pixelData.empty()) return;

    size_t count = std::min(frameA.pixelData.size(), frameB.pixelData.size());
    double alpha = std::clamp(progress, 0.0, 1.0);

    for (size_t i = 0; i < count; ++i) {
        outFrame.pixelData[i] = static_cast<uint8_t>(frameA.pixelData[i] * (1.0 - alpha) + frameB.pixelData[i] * alpha);
    }
}

void DipToBlackTransition::Apply(const MediaFrame& frameA, const MediaFrame& frameB, double progress, MediaFrame& outFrame) {
    double alpha = std::clamp(progress, 0.0, 1.0);
    if (alpha < 0.5) {
        outFrame = frameA;
        double darkness = 1.0 - (alpha * 2.0);
        for (size_t i = 0; i < outFrame.pixelData.size(); ++i) {
            outFrame.pixelData[i] = static_cast<uint8_t>(outFrame.pixelData[i] * darkness);
        }
    } else {
        outFrame = frameB;
        double lightness = (alpha - 0.5) * 2.0;
        for (size_t i = 0; i < outFrame.pixelData.size(); ++i) {
            outFrame.pixelData[i] = static_cast<uint8_t>(outFrame.pixelData[i] * lightness);
        }
    }
}

} // namespace jaguar
