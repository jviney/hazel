# Hazel

This repository is following the game engine series by [TheCherno](https://www.youtube.com/user/TheChernoProject).

It is being developed on macOS using VS Code with a CMake build system, instead of Windows using Visual Studio with Premake.

Other differences:

- snake case instead of camel case (personal preference)
- slightly different directory layout
- `.hpp` instead of `.h` for header files

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
