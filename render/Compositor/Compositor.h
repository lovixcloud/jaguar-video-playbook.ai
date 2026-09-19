#pragma once

#include "capture/Synchronization/AudioVideoSynchronizer.h"
#include "core/Project/ProjectModel.h"
#include "render/Effects/IVideoEffect.h"

namespace jaguar {

class Compositor {
public:
    Compositor();

    MediaFrame CompositeFrame(const Project& project, double timeSeconds);

private:
    ColorAdjustEffect m_colorEffect;
    GrayscaleEffect m_grayscaleEffect;
    VignetteEffect m_vignetteEffect;
};

} // namespace jaguar
