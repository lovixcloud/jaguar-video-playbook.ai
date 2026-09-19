#pragma once

#include "core/Project/ProjectModel.h"
#include <string>

namespace jaguar {

class TimelineEngine {
public:
    TimelineEngine() = default;

    static bool AddClipToTrack(Project& project, const std::string& trackId, const TimelineClip& clip);
    static bool RemoveClip(Project& project, const std::string& clipId);
    static bool MoveClip(Project& project, const std::string& clipId, double newStartSeconds);
    static bool TrimClip(Project& project, const std::string& clipId, double newStartSeconds, double newDurationSeconds);
    static bool SplitClip(Project& project, const std::string& clipId, double splitTimeSeconds);
    static bool DuplicateClip(Project& project, const std::string& clipId);

    static bool AddMarker(Project& project, const std::string& name, double timeSeconds, const std::string& colorHex);
    static bool RemoveMarker(Project& project, const std::string& markerId);
};

} // namespace jaguar
