#include "TimelineEngine.h"
#include <algorithm>

namespace jaguar {

bool TimelineEngine::AddClipToTrack(Project& project, const std::string& trackId, const TimelineClip& clip) {
    for (auto& track : project.tracks) {
        if (track.id == trackId) {
            track.clips.push_back(clip);
            return true;
        }
    }
    return false;
}

bool TimelineEngine::RemoveClip(Project& project, const std::string& clipId) {
    for (auto& track : project.tracks) {
        auto it = std::remove_if(track.clips.begin(), track.clips.end(),
            [&clipId](const TimelineClip& c) { return c.id == clipId; });
        if (it != track.clips.end()) {
            track.clips.erase(it, track.clips.end());
            return true;
        }
    }
    return false;
}

bool TimelineEngine::MoveClip(Project& project, const std::string& clipId, double newStartSeconds) {
    for (auto& track : project.tracks) {
        for (auto& clip : track.clips) {
            if (clip.id == clipId) {
                clip.timelineStartSeconds = std::max(0.0, newStartSeconds);
                return true;
            }
        }
    }
    return false;
}

bool TimelineEngine::TrimClip(Project& project, const std::string& clipId, double newStartSeconds, double newDurationSeconds) {
    for (auto& track : project.tracks) {
        for (auto& clip : track.clips) {
            if (clip.id == clipId) {
                clip.timelineStartSeconds = std::max(0.0, newStartSeconds);
                clip.durationSeconds = std::max(0.1, newDurationSeconds);
                return true;
            }
        }
    }
    return false;
}

bool TimelineEngine::SplitClip(Project& project, const std::string& clipId, double splitTimeSeconds) {
    for (auto& track : project.tracks) {
        for (size_t i = 0; i < track.clips.size(); ++i) {
            auto& clip = track.clips[i];
            if (clip.id == clipId) {
                if (splitTimeSeconds <= clip.timelineStartSeconds ||
                    splitTimeSeconds >= (clip.timelineStartSeconds + clip.durationSeconds)) {
                    return false;
                }

                double firstDuration = splitTimeSeconds - clip.timelineStartSeconds;
                double secondDuration = clip.durationSeconds - firstDuration;

                TimelineClip secondClip = clip;
                secondClip.id = clip.id + "_part2";
                secondClip.timelineStartSeconds = splitTimeSeconds;
                secondClip.mediaStartSeconds = clip.mediaStartSeconds + firstDuration;
                secondClip.durationSeconds = secondDuration;

                clip.durationSeconds = firstDuration;
                track.clips.insert(track.clips.begin() + i + 1, secondClip);
                return true;
            }
        }
    }
    return false;
}

bool TimelineEngine::DuplicateClip(Project& project, const std::string& clipId) {
    for (auto& track : project.tracks) {
        for (size_t i = 0; i < track.clips.size(); ++i) {
            if (track.clips[i].id == clipId) {
                TimelineClip dup = track.clips[i];
                dup.id = clipId + "_copy";
                dup.timelineStartSeconds += dup.durationSeconds;
                track.clips.insert(track.clips.begin() + i + 1, dup);
                return true;
            }
        }
    }
    return false;
}

bool TimelineEngine::AddMarker(Project& project, const std::string& name, double timeSeconds, const std::string& colorHex) {
    TimelineMarker m;
    m.id = "marker_" + std::to_string(project.markers.size() + 1);
    m.name = name;
    m.timeSeconds = timeSeconds;
    m.colorHex = colorHex;
    project.markers.push_back(m);
    return true;
}

bool TimelineEngine::RemoveMarker(Project& project, const std::string& markerId) {
    auto it = std::remove_if(project.markers.begin(), project.markers.end(),
        [&markerId](const TimelineMarker& m) { return m.id == markerId; });
    if (it != project.markers.end()) {
        project.markers.erase(it, project.markers.end());
        return true;
    }
    return false;
}

} // namespace jaguar
