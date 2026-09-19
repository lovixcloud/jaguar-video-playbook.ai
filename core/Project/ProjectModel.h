#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace jaguar {

struct EffectParameters {
    std::string effectId;
    double brightness{ 0.0 };
    double contrast{ 1.0 };
    double saturation{ 1.0 };
    double exposure{ 0.0 };
    double blurRadius{ 0.0 };
    double opacity{ 1.0 };
    bool grayscale{ false };
    double vignette{ 0.0 };
};

struct TextOverlay {
    std::string id;
    std::string text;
    std::string fontName{ "Arial" };
    double fontSize{ 32.0 };
    std::string colorHex{ "#FFFFFF" };
    double posX{ 0.5 };
    double posY{ 0.5 };
    double startTimeSeconds{ 0.0 };
    double durationSeconds{ 5.0 };
};

struct TimelineClip {
    std::string id;
    std::string mediaAssetId;
    std::string filePath;
    double timelineStartSeconds{ 0.0 };
    double mediaStartSeconds{ 0.0 };
    double durationSeconds{ 10.0 };
    double playheadSpeed{ 1.0 };
    double volume{ 1.0 };
    double fadeInSeconds{ 0.0 };
    double fadeOutSeconds{ 0.0 };
    double opacity{ 1.0 };
    double posX{ 0.0 };
    double posY{ 0.0 };
    double scale{ 1.0 };
    double rotationDegrees{ 0.0 };
    std::vector<EffectParameters> effects;
    std::vector<TextOverlay> textOverlays;
};

enum class TrackType {
    Video,
    Audio,
    Text,
    Overlay
};

struct TimelineTrack {
    std::string id;
    std::string name;
    TrackType type{ TrackType::Video };
    bool locked{ false };
    bool hidden{ false };
    bool muted{ false };
    bool solo{ false };
    std::vector<TimelineClip> clips;
};

struct TimelineMarker {
    std::string id;
    std::string name;
    double timeSeconds{ 0.0 };
    std::string colorHex{ "#FF0000" };
};

struct MediaAsset {
    std::string id;
    std::string filePath;
    std::string fileName;
    double durationSeconds{ 0.0 };
    int width{ 0 };
    int height{ 0 };
    double fps{ 0.0 };
    int sampleRate{ 0 };
    int channels{ 0 };
    std::string codec;
    int64_t fileSizeBytes{ 0 };
    std::string thumbnailPath;
};

struct Project {
    int formatVersion{ 1 };
    std::string name{ "Untitled Project" };
    std::string projectFilePath;
    int canvasWidth{ 1920 };
    int canvasHeight{ 1080 };
    double fps{ 60.0 };
    double totalDurationSeconds{ 0.0 };
    std::vector<MediaAsset> mediaAssets;
    std::vector<TimelineTrack> tracks;
    std::vector<TimelineMarker> markers;
};

class ProjectSerializer {
public:
    static bool SaveToFile(const Project& project, const std::string& filePath);
    static bool LoadFromFile(const std::string& filePath, Project& outProject);
};

class ProjectManager {
public:
    static ProjectManager& Instance();

    void CreateNewProject(const std::string& name = "Untitled Project");
    bool OpenProject(const std::string& filePath);
    bool SaveProject(const std::string& filePath = "");
    bool SaveAutosaveCopy(const std::string& autosaveDir);

    Project& GetCurrentProject() { return m_currentProject; }
    const Project& GetCurrentProject() const { return m_currentProject; }
    bool IsDirty() const { return m_isDirty; }
    void MarkDirty(bool dirty = true) { m_isDirty = dirty; }

private:
    ProjectManager() { CreateNewProject(); }

    Project m_currentProject;
    bool m_isDirty{ false };
};

} // namespace jaguar
