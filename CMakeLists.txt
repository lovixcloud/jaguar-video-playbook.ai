cmake_minimum_required(VERSION 3.20)
project(Jaguar VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /EHsc /utf-8)
else()
    add_compile_options(-Wall -Wextra -std=c++20)
    add_link_options(-municode -static -static-libgcc -static-libstdc++)
endif()

include_directories(
    .
    app
    core
    capture
    media
    render
    editor
    ui
    export
)

set(CORE_SOURCES
    core/Logging/Logger.cpp
    core/Logging/Logger.h
    core/Settings/SettingsManager.cpp
    core/Settings/SettingsManager.h
    core/Project/ProjectModel.cpp
    core/Project/ProjectModel.h
    core/Tasks/TaskScheduler.cpp
    core/Tasks/TaskScheduler.h
    core/Commands/CommandHistory.cpp
    core/Commands/CommandHistory.h
)

set(CAPTURE_SOURCES
    capture/Synchronization/AudioVideoSynchronizer.cpp
    capture/Synchronization/AudioVideoSynchronizer.h
    capture/Screen/ScreenCapture.cpp
    capture/Screen/ScreenCapture.h
    capture/Webcam/WebcamCapture.cpp
    capture/Webcam/WebcamCapture.h
    capture/Microphone/MicrophoneCapture.cpp
    capture/Microphone/MicrophoneCapture.h
    capture/SystemAudio/SystemAudioCapture.cpp
    capture/SystemAudio/SystemAudioCapture.h
)

set(MEDIA_SOURCES
    media/Decoder/MediaDecoder.cpp
    media/Decoder/MediaDecoder.h
    media/Encoder/MediaEncoder.cpp
    media/Encoder/MediaEncoder.h
    media/Cache/MediaCache.cpp
    media/Cache/MediaCache.h
    media/Waveform/WaveformGenerator.cpp
    media/Waveform/WaveformGenerator.h
)

set(RENDER_SOURCES
    render/Effects/IVideoEffect.cpp
    render/Effects/IVideoEffect.h
    render/Transitions/ITransition.cpp
    render/Transitions/ITransition.h
    render/Compositor/Compositor.cpp
    render/Compositor/Compositor.h
)

set(EDITOR_SOURCES
    editor/Timeline/TimelineEngine.cpp
    editor/Timeline/TimelineEngine.h
    editor/Preview/PreviewEngine.cpp
    editor/Preview/PreviewEngine.h
)

set(EXPORT_SOURCES
    export/ExportEngine/ExportJob.cpp
    export/ExportEngine/ExportJob.h
    export/ExportQueue/ExportQueue.cpp
    export/ExportQueue/ExportQueue.h
)

set(UI_SOURCES
    ui/MainWindow/StatusSystem.cpp
    ui/MainWindow/StatusSystem.h
    ui/MainWindow/JaguarStudioWindow.cpp
    ui/MainWindow/JaguarStudioWindow.h
    ui/MainWindow/UITheme.h
    ui/MainWindow/Navigation.h
)

add_executable(Jaguar WIN32
    app/JaguarMain.cpp
    app/JaguarApp.cpp
    app/JaguarApp.h
    ${CORE_SOURCES}
    ${CAPTURE_SOURCES}
    ${MEDIA_SOURCES}
    ${RENDER_SOURCES}
    ${EDITOR_SOURCES}
    ${EXPORT_SOURCES}
    ${UI_SOURCES}
)

add_executable(JaguarTests
    tests/JaguarTests.cpp
    ${CORE_SOURCES}
    ${CAPTURE_SOURCES}
    ${MEDIA_SOURCES}
    ${RENDER_SOURCES}
    ${EDITOR_SOURCES}
    ${EXPORT_SOURCES}
)

if(WIN32)
    target_link_libraries(Jaguar PRIVATE
        user32
        gdi32
        comctl32
        ole32
        shell32
        uuid
        mfplat
        mfreadwrite
        mfuuid
        dwmapi
    )
    target_link_libraries(JaguarTests PRIVATE
        user32
        gdi32
        comctl32
        ole32
        shell32
        uuid
        mfplat
        mfreadwrite
        mfuuid
        dwmapi
    )
endif()

install(TARGETS Jaguar DESTINATION bin)
