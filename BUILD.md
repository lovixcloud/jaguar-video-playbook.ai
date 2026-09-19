# Building Jaguar Studio

## Prerequisites

- **Windows:** Visual Studio 2022 (Desktop development with C++ workload) or CMake 3.20+ with MSVC/MinGW-w64 x64 toolchain.
- **Linux Cross-Compilation:** CMake, `x86_64-w64-mingw32-g++` (GCC 13+), Ninja.

## Build Commands

### CMake (Command Line)
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### Visual Studio
Open the project directory in Visual Studio 2022 or open `CMakeLists.txt` via CMake Integration. Select `x64-Release` build configuration and press Build.

Output binary location: `build/Jaguar.exe`.
