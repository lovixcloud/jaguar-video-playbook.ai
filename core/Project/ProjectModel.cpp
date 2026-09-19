#include "ProjectModel.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace jaguar {

bool ProjectSerializer::SaveToFile(const Project& project, const std::string& filePath) {
    std::filesystem::path p(filePath);
    if (p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }

    std::ofstream file(filePath);
    if (!file.is_open()) return false;

    file << "{\n";
    file << "  \"formatVersion\": " << project.formatVersion << ",\n";
    file << "  \"name\": \"" << project.name << "\",\n";
    file << "  \"canvasWidth\": " << project.canvasWidth << ",\n";
    file << "  \"canvasHeight\": " << project.canvasHeight << ",\n";
    file << "  \"fps\": " << project.fps << ",\n";
    file << "  \"mediaCount\": " << project.mediaAssets.size() << ",\n";
    file << "  \"media\": [\n";
    for (size_t i = 0; i < project.mediaAssets.size(); ++i) {
        const auto& m = project.mediaAssets[i];
        file << "    {\"id\":\"" << m.id << "\", \"path\":\"" << m.filePath << "\", \"duration\":" << m.durationSeconds << "}";
        if (i + 1 < project.mediaAssets.size()) file << ",";
        file << "\n";
    }
    file << "  ],\n";
    file << "  \"tracksCount\": " << project.tracks.size() << ",\n";
    file << "  \"tracks\": [\n";
    for (size_t i = 0; i < project.tracks.size(); ++i) {
        const auto& tr = project.tracks[i];
        file << "    {\"id\":\"" << tr.id << "\", \"name\":\"" << tr.name << "\", \"clipsCount\":" << tr.clips.size() << "}";
        if (i + 1 < project.tracks.size()) file << ",";
        file << "\n";
    }
    file << "  ]\n";
    file << "}\n";

    return true;
}

bool ProjectSerializer::LoadFromFile(const std::string& filePath, Project& outProject) {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    outProject = Project();
    outProject.projectFilePath = filePath;
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("\"name\":") != std::string::npos) {
            auto firstQuote = line.find('"', line.find(':'));
            auto lastQuote = line.rfind('"');
            if (firstQuote != std::string::npos && lastQuote > firstQuote) {
                outProject.name = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
            }
        }
    }
    return true;
}

ProjectManager& ProjectManager::Instance() {
    static ProjectManager instance;
    return instance;
}

void ProjectManager::CreateNewProject(const std::string& name) {
    m_currentProject = Project();
    m_currentProject.name = name;

    TimelineTrack v1; v1.id = "tr_v1"; v1.name = "Video Track 1"; v1.type = TrackType::Video;
    TimelineTrack v2; v2.id = "tr_v2"; v2.name = "Video Track 2"; v2.type = TrackType::Video;
    TimelineTrack ov; ov.id = "tr_ov"; ov.name = "Overlay Track"; ov.type = TrackType::Overlay;
    TimelineTrack a1; a1.id = "tr_a1"; a1.name = "Audio Track 1"; a1.type = TrackType::Audio;
    TimelineTrack a2; a2.id = "tr_a2"; a2.name = "Audio Track 2"; a2.type = TrackType::Audio;
    TimelineTrack txt; txt.id = "tr_txt"; txt.name = "Text Track"; txt.type = TrackType::Text;

    m_currentProject.tracks.push_back(v1);
    m_currentProject.tracks.push_back(v2);
    m_currentProject.tracks.push_back(ov);
    m_currentProject.tracks.push_back(a1);
    m_currentProject.tracks.push_back(a2);
    m_currentProject.tracks.push_back(txt);

    m_isDirty = false;
}

bool ProjectManager::OpenProject(const std::string& filePath) {
    Project p;
    if (ProjectSerializer::LoadFromFile(filePath, p)) {
        m_currentProject = p;
        m_isDirty = false;
        return true;
    }
    return false;
}

bool ProjectManager::SaveProject(const std::string& filePath) {
    std::string path = filePath.empty() ? m_currentProject.projectFilePath : filePath;
    if (path.empty()) {
        path = m_currentProject.name + ".jaguar";
    }
    if (ProjectSerializer::SaveToFile(m_currentProject, path)) {
        m_currentProject.projectFilePath = path;
        m_isDirty = false;
        return true;
    }
    return false;
}

bool ProjectManager::SaveAutosaveCopy(const std::string& autosaveDir) {
    std::string autosavePath = autosaveDir + "/autosave_" + m_currentProject.name + ".jaguar";
    return ProjectSerializer::SaveToFile(m_currentProject, autosavePath);
}

} // namespace jaguar
