# Jaguar Media & Capture Pipeline

## Frame Pipeline Flow

```
[Screen / Webcam / Audio Source]
            │
            ▼
 [AudioVideoSynchronizer]  (Timestamp Alignment)
            │
            ▼
    [Processing Queue]
            │
            ▼
   [Compositor / Effects]
            │
            ▼
     [MediaEncoder]
            │
            ▼
  [Output Container (.mp4)]
```
