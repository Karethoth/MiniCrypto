#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check for required tools
for cmd in rm curl unzip git perl nasm; do
    if ! command_exists $cmd; then
        echo "Error: $cmd is not installed." >&2
        exit 1
    fi
done

# Clean
rm -rf 3rdparty/SDL2
rm -rf 3rdparty/imgui
rm -rf 3rdparty/imnodes
rm -rf 3rdparty/openssl

# SDL2
SDL2_URL="http://www.libsdl.org/release/SDL2-devel-2.0.16-VC.zip"
SDL2_ZIP="SDL2.zip"
curl -L $SDL2_URL --output $SDL2_ZIP
unzip $SDL2_ZIP -d 3rdparty/SDL2/
mv 3rdparty/SDL2/SDL2*/* 3rdparty/SDL2/
rm -rf $SDL2_ZIP 3rdparty/SDL2/SDL2-*

# IMGUI
git clone --depth=1 --branch docking https://github.com/ocornut/imgui 3rdparty/imgui

# ImNodes
git clone --depth=1 https://github.com/Nelarius/imnodes 3rdparty/imnodes

# OpenSSL
git clone --depth=1 https://github.com/openssl/openssl 3rdparty/openssl
cd 3rdparty/openssl
git config core.autocrlf false
git config core.eol lf
git checkout .

echo ""
echo "Automatic dependencies downloaded successfully."
echo ""
echo "Download GLEW binaries from https://glew.sourceforge.net/ and extract them to 3rdparty/glew"
echo ""
echo "Please go open 3rdparty/openssl dir in the 'x64 Native Tools Command Prompt for VS 2022' as an Administrator and run:"
echo "perl Configure VC-WIN64A"
echo "nmake"
echo "nmake install"
echo ""
read -p "Press Enter to exit this script..."
