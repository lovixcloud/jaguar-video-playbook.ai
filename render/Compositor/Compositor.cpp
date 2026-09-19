#include "Compositor.h"
#include <algorithm>

namespace jaguar {

Compositor::Compositor() = default;

MediaFrame Compositor::CompositeFrame(const Project& project, double timeSeconds) {
    MediaFrame result;
    result.type = MediaFrame::Type::Video;
    result.width = project.canvasWidth;
    result.height = project.canvasHeight;
    result.timestampMs = static_cast<int64_t>(timeSeconds * 1000.0);
    result.pixelData.resize(result.width * result.height * 4, 30);

    for (const auto& track : project.tracks) {
        if (track.hidden || track.type != TrackType::Video) continue;

        for (const auto& clip : track.clips) {
            if (timeSeconds >= clip.timelineStartSeconds &&
                timeSeconds < (clip.timelineStartSeconds + clip.durationSeconds)) {

                uint8_t fillVal = 180;
                for (size_t i = 0; i < result.pixelData.size(); i += 4) {
                    result.pixelData[i] = fillVal;
                    result.pixelData[i + 1] = fillVal;
                    result.pixelData[i + 2] = fillVal;
                }

                for (const auto& fx : clip.effects) {
                    m_colorEffect.Apply(result, fx);
                    m_grayscaleEffect.Apply(result, fx);
                    m_vignetteEffect.Apply(result, fx);
                }
            }
        }
    }

    return result;
}

} // namespace jaguar
