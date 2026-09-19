# Jaguar Studio Development Guide

## Code Conventions

- **C++ Standard:** C++20.
- **Code Style:** RAII, smart pointers (`std::unique_ptr`, `std::shared_ptr`), `const` correctness, standard exception safety.
- **Thread Safety:** Never run encoding, heavy decoding, waveform generation, or file I/O on the UI thread. Use `TaskScheduler` for asynchronous worker threads.
- **Error Handling:** Centralized logging via `Logger::Instance()` and status notifications via `StatusSystem::Instance()`.
