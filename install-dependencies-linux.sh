#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

command_exists() {
    command -v "$1" >/dev/null 2>&1
}

package_installed() {
    dpkg -s "$1" >/dev/null 2>&1
}

# Check for required tools
for cmd in rm git curl unzip; do
    if ! command_exists $cmd; then
        echo "Error: $cmd is not installed." >&2
        exit 1
    fi
done

# Clean
rm -rf 3rdparty/imgui
rm -rf 3rdparty/imnodes

# SDL2
if ! package_installed libsdl2-dev; then
    echo "Installing SDL2 development libraries..."
    sudo apt-get install -y libsdl2-dev
else
    echo "SDL2 development libraries are already installed."
fi

# IMGUI
if [ ! -d "3rdparty/imgui" ]; then
    echo "Cloning IMGUI..."
    git clone --depth=1 --branch docking https://github.com/ocornut/imgui 3rdparty/imgui
else
    echo "IMGUI is already cloned."
fi

# ImNodes
if [ ! -d "3rdparty/imnodes" ]; then
    echo "Cloning ImNodes..."
    git clone --depth=1 https://github.com/Nelarius/imnodes 3rdparty/imnodes
else
    echo "ImNodes is already cloned."
fi

# Curl
if ! package_installed libcurl4-openssl-dev; then
    echo "Installing cURL development libraries..."
    sudo apt-get install -y libcurl4-openssl-dev
else
    echo "cURL development libraries are already installed."
fi

# OpenSSL
if ! package_installed libssl-dev; then
    echo "Installing OpenSSL development libraries..."
    sudo apt-get install -y libssl-dev
else
    echo "OpenSSL development libraries are already installed."
fi

# GLEW
if ! package_installed libglew-dev; then
    echo "Installing GLEW development libraries..."
    sudo apt-get install -y libglew-dev
else
    echo "GLEW development libraries are already installed."
fi

echo ""
echo "Dependencies installed successfully."
echo ""
read -p "Press Enter to exit this script..."

