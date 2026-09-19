#pragma once

#include "capture/Synchronization/AudioVideoSynchronizer.h"
#include "core/Project/ProjectModel.h"

namespace jaguar {

class IVideoEffect {
public:
    virtual ~IVideoEffect() = default;
    virtual void Apply(MediaFrame& frame, const EffectParameters& params) = 0;
};

class ColorAdjustEffect : public IVideoEffect {
public:
    void Apply(MediaFrame& frame, const EffectParameters& params) override;
};

class GrayscaleEffect : public IVideoEffect {
public:
    void Apply(MediaFrame& frame, const EffectParameters& params) override;
};

class VignetteEffect : public IVideoEffect {
public:
    void Apply(MediaFrame& frame, const EffectParameters& params) override;
};

} // namespace jaguar
