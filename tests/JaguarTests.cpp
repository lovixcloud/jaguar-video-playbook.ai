#include "core/Project/ProjectModel.h"
#include "core/Commands/CommandHistory.h"
#include "editor/Timeline/TimelineEngine.h"
#include "media/Cache/MediaCache.h"
#include "media/Waveform/WaveformGenerator.h"
#include "export/ExportQueue/ExportQueue.h"
#include "core/Tasks/TaskScheduler.h"
#include <iostream>
#include <cassert>
#include <filesystem>

using namespace jaguar;

class DummyCommand : public ICommand {
public:
    DummyCommand(int& val) : m_val(val) {}
    void Execute() override { m_val += 10; }
    void Undo() override { m_val -= 10; }
    std::string GetName() const override { return "DummyCommand"; }
private:
    int& m_val;
};

void TestProjectModel() {
    Project p;
    p.name = "Test Project";
    p.canvasWidth = 1920;
    p.canvasHeight = 1080;
    p.fps = 60.0;

    std::string testPath = "test_proj.jaguar";
    bool saved = ProjectSerializer::SaveToFile(p, testPath);
    (void)saved;
    assert(saved);

    Project loaded;
    bool loadedOk = ProjectSerializer::LoadFromFile(testPath, loaded);
    (void)loadedOk;
    assert(loadedOk);
    assert(loaded.name == "Test Project");

    std::filesystem::remove(testPath);
    std::cout << "[PASS] TestProjectModel" << std::endl;
}

void TestTimelineOperations() {
    Project p;
    ProjectManager::Instance().CreateNewProject("Timeline Test");
    p = ProjectManager::Instance().GetCurrentProject();

    assert(!p.tracks.empty());
    std::string trackId = p.tracks[0].id;

    TimelineClip clip;
    clip.id = "clip_1";
    clip.timelineStartSeconds = 0.0;
    clip.durationSeconds = 10.0;

    bool added = TimelineEngine::AddClipToTrack(p, trackId, clip);
    (void)added;
    assert(added);

    bool moved = TimelineEngine::MoveClip(p, "clip_1", 5.0);
    (void)moved;
    assert(moved);

    bool split = TimelineEngine::SplitClip(p, "clip_1", 7.0);
    (void)split;
    assert(split);

    bool trimmed = TimelineEngine::TrimClip(p, "clip_1", 5.0, 1.5);
    (void)trimmed;
    assert(trimmed);

    bool removed = TimelineEngine::RemoveClip(p, "clip_1");
    (void)removed;
    assert(removed);

    std::cout << "[PASS] TestTimelineOperations" << std::endl;
}

void TestUndoRedo() {
    int val = 0;
    CommandHistory::Instance().Clear();
    assert(!CommandHistory::Instance().CanUndo());

    CommandHistory::Instance().ExecuteCommand(std::make_unique<DummyCommand>(val));
    assert(val == 10);
    assert(CommandHistory::Instance().CanUndo());

    CommandHistory::Instance().Undo();
    assert(val == 0);
    assert(CommandHistory::Instance().CanRedo());

    CommandHistory::Instance().Redo();
    assert(val == 10);

    std::cout << "[PASS] TestUndoRedo" << std::endl;
}

void TestMediaCacheAndWaveform() {
    MediaCache::Instance().SetCacheDir("test_cache");
    std::vector<float> peaks = { 0.1f, 0.5f, 0.9f, 0.3f };
    MediaCache::Instance().StoreWaveform("media_1", peaks);

    assert(MediaCache::Instance().HasWaveform("media_1"));
    auto cached = MediaCache::Instance().GetWaveform("media_1");
    assert(cached.size() == 4);
    assert(cached[1] == 0.5f);

    std::cout << "[PASS] TestMediaCacheAndWaveform" << std::endl;
}

void TestExportQueue() {
    TaskScheduler::Instance().Initialize(2);
    Project p;
    p.totalDurationSeconds = 1.0;
    p.fps = 10.0;

    EncoderConfig config;
    config.outputPath = "test_export.mp4";
    config.width = 640;
    config.height = 360;
    config.fps = 10;

    std::string jobId = ExportQueue::Instance().AddJob(p, config);
    assert(!jobId.empty());

    auto jobs = ExportQueue::Instance().GetAllJobs();
    assert(!jobs.empty());

    std::filesystem::remove("test_export.mp4");
    std::cout << "[PASS] TestExportQueue" << std::endl;
}

int wmain() {
    std::cout << "=== Running Jaguar Studio Unit Tests ===" << std::endl;
    TestProjectModel();
    TestTimelineOperations();
    TestUndoRedo();
    TestMediaCacheAndWaveform();
    TestExportQueue();
    std::cout << "=== All Unit Tests Passed Successfully ===" << std::endl;
    return 0;
}
