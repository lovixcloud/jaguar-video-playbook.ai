#pragma once

#include "capture/Synchronization/AudioVideoSynchronizer.h"

namespace jaguar {

enum class TransitionType {
    Cut,
    Fade,
    Crossfade,
    DipToBlack,
    DipToWhite,
    Slide
};

class ITransition {
public:
    virtual ~ITransition() = default;
    virtual void Apply(const MediaFrame& frameA, const MediaFrame& frameB, double progress, MediaFrame& outFrame) = 0;
};

class CrossfadeTransition : public ITransition {
public:
    void Apply(const MediaFrame& frameA, const MediaFrame& frameB, double progress, MediaFrame& outFrame) override;
};

class DipToBlackTransition : public ITransition {
public:
    void Apply(const MediaFrame& frameA, const MediaFrame& frameB, double progress, MediaFrame& outFrame) override;
};

} // namespace jaguar
