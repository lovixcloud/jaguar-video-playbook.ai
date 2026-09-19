# Jaguar Studio — Native Windows Video Recorder & Editor

Jaguar Studio is a modern, high-performance, native Windows desktop application for screen recording, webcam capture, multi-source audio recording, real-time timeline editing, video compositing, and video export.

Built strictly in native C++20 for Windows using modern DirectX/Direct3D, Windows Media Foundation, WASAPI, and Win32 desktop APIs.

## Features

- **Screen & Display Capture:** Multi-monitor screen capture, specific application window capture, custom rectangular regions.
- **Webcam Integration:** Live webcam preview, picture-in-picture, crop, opacity, and positioning.
- **Audio Capture & Loopback:** Microphone input and WASAPI system audio loopback with live audio level meters and synchronization.
- **Non-Destructive Timeline Editor:** Multi-track video/audio/overlay timeline, frame-accurate clip trim, split, move, duplicate, and markers.
- **Video Effects & Transitions:** Color adjustment, contrast/brightness, saturation, grayscale, vignette, crossfade, and dip to black.
- **GPU Preview & Compositor:** Real-time multi-layer compositor.
- **Export Queue Engine:** Multi-job background export queue with progress tracking and ETA calculations.
- **Project File Format:** Versioned structured `.jaguar` project format.
- **Local-First & Safe:** Local processing, offline support, automatic recovery snapshots.

## Building Jaguar Studio

See `BUILD.md` for detailed instructions for Visual Studio MSVC and CMake/MinGW-w64.
