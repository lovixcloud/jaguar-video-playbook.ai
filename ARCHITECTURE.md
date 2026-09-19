# Jaguar Studio Architecture

Jaguar Studio follows a modular, decoupled C++20 native desktop architecture.

## Subsystems

- `app/`: Application entry point (`wWinMain`), application life-cycle and main message loop.
- `core/`: Log subsystem (`Logger`), thread-safe settings (`SettingsManager`), `.jaguar` project serialization (`ProjectModel`), task scheduler (`TaskScheduler`), command history for undo/redo (`CommandHistory`).
- `capture/`: Display/screen capture (`ScreenCapture`), webcam stream (`WebcamCapture`), microphone input (`MicrophoneCapture`), system audio loopback (`SystemAudioCapture`), and timestamp normalizer (`AudioVideoSynchronizer`).
- `media/`: Media discovery & decoding (`MediaDecoder`), frame encoding (`MediaEncoder`), disk cache (`MediaCache`), background waveform generator (`WaveformGenerator`).
- `render/`: Extensible video effects (`IVideoEffect`), transition engine (`ITransition`), multi-layer scene compositor (`Compositor`).
- `editor/`: Timeline operations engine (`TimelineEngine`), playback controller & preview engine (`PreviewEngine`).
- `export/`: Asynchronous export job (`ExportJob`), multi-job queue manager (`ExportQueue`).
- `ui/`: Native Win32 dark professional GUI (`JaguarStudioWindow`, `UITheme`, `Navigation`, `StatusSystem`).
