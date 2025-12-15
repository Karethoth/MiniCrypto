# MiniCrypto
Practice environment for the [CryptoPals](https://cryptopals.com) challenges.

*Thanks to [shxy0](https://github.com/shxy0) for setting up an [example project](https://github.com/shxy0/SDLImGui) for CMake+ImGui+SDL2.*

## Dependencies
- C++20 Compiler
- CMake
- [ImGui](https://github.com/ocornut/imgui)
- [imnodes](https://github.com/Nelarius/imnodes)
- [SDL2](https://www.libsdl.org)
- OpenSSL
- cURL

## Windows
### Installing dependencies on Windows
- Having git bash, cygwin, WSL or something to run `wget`, `git` and `bash` is required.
- Run `./install-dependencies-win.sh`

### Development Environment
#### VS Code
*A proper Visual Studio may still be needed for compiling OpenSSL*
- Extensions:
  - C/C++
  - CMake
  - CMake Tools
- Initial setup:
  - Open the project folder in VS Code
  - Hit `Ctrl+Shift+P` and run "`CMake: Configure`"
  - If asked to select a kit, pick the compiler you want to use
  - Hit `Ctrl+F5` and select the target

#### Visual Studio 2022 Preview
- Open the directory as a local folder in VS2022
- Wait for `CMake generation finished.` message in the `Output` tab
- From the top bar, using the arrow next to `Select Startup Item` pick `minicrypto.exe`
- Hit `Ctrl+F5`

## Linux
- Install the system dependencies (SDL2, OpenGL, OpenSSL, cURL, GLEW, X11 Xi)
- Run `./install-dependencies-linux.sh` to populate `3rdparty/imgui` and `3rdparty/imnodes`
- Basic CMake process
  - `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`
  - `cmake --build build -j`
  - Run: `./build/minicrypto`
  - Run tests: `ctest --test-dir build` (or `./build/tests/tests -s`)

### Bazzite (Fedora Atomic)
Recommended: build inside a Toolbox (keeps the host immutable).

```sh
toolbox create -c minicrypto
toolbox enter minicrypto
sudo dnf install -y gcc-c++ cmake make git pkgconf-pkg-config \
  SDL2-devel mesa-libGL-devel libX11-devel libXi-devel \
  libepoxy-devel libcurl-devel openssl-devel

cd /path/to/MiniCrypto
./install-dependencies-linux.sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/minicrypto
```

