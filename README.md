# MiniCrypto
Practice environment for the [CryptoPals](https://cryptopals.com) challenges.

*Thanks to [shxy0](https://github.com/shxy0) for setting up an [example project](https://github.com/shxy0/SDLImGui) for CMake+ImGui+SDL2.*

## Dependencies
- C++17 Compiler
- CMake
- [ImGui](https://github.com/ocornut/imgui)
- [imgui-node-editor](https://github.com/thedmd/imgui-node-editor)
- [SDL2](https://www.libsdl.org)

## Windows
### Installing dependencies on Windows
- Having git bash, cygwin, WSL or something to run `wget`, `git` and `bash` is required.
- Run `./install-dependencies-win.sh`

### Development Environment
#### VS Code
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
- Install SDL2 development libraries
- Run `./install-dependencies-linux.sh` to populate `3rdparty/imgui` and `3rdparty/imgui-node-editor`
- Basic CMake process
  - `mkdir build && cd build && cmake .. && make`
  - You can run the tests with `make test`
    - For more verbose output you can run
      - `./tests/tests -s`

# Notes
- Bug in imgui-node-editor after ImGui update:
  - Possible error message: `..\3rdparty\imgui-node-editor\imgui_canvas.cpp(104): error C2660: 'ImGui::IsClippedEx': function does not take 3 arguments`
  - Fix: Open the `imgui_canvas.cpp` file and remove the third argument from the mentioned line.

