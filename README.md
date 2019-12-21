# Hazel

This repository is following the
[game engine series](https://www.youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT)
by [TheCherno](https://www.youtube.com/user/TheChernoProject).

It is being developed on macOS using VS Code with a CMake build system, instead of Windows using Visual Studio with Premake.

Changes for macOS and CMake:

- `glfwWindowHint()` calls to get macOS to load OpenGL 4.1.
- Correct scaling for retina displays with ImGui.
- ImGui git submodule uses a fork with CMake support.

Please support TheCherno on [Patreon](https://patreon.com/thecherno).

## Building

```
brew install glfw

git clone --recursive https://github.com/jviney/hazel
cd hazel/build
cmake ..
make -j2
./sandbox
```

### Create Xcode project

```
mkdir build-xcode
cd build-xcode
cmake -G Xcode ..
open hazel.xcodeproj
```
