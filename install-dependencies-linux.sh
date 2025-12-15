#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

command_exists() {
    command -v "$1" >/dev/null 2>&1
}

detect_pkg_manager() {
    if command_exists apt-get; then
        echo "apt"
    elif command_exists dnf; then
        echo "dnf"
    elif command_exists rpm-ostree; then
        echo "rpm-ostree"
    else
        echo "unknown"
    fi
}

package_installed() {
    local mgr
    mgr="$(detect_pkg_manager)"

    if [ "$mgr" = "apt" ]; then
        dpkg -s "$1" >/dev/null 2>&1
    elif [ "$mgr" = "dnf" ]; then
        rpm -q "$1" >/dev/null 2>&1
    else
        return 1
    fi
}

install_packages() {
    local mgr
    mgr="$(detect_pkg_manager)"

    if [ "$mgr" = "apt" ]; then
        sudo apt-get update
        sudo apt-get install -y "$@"
    elif [ "$mgr" = "dnf" ]; then
        sudo dnf install -y "$@"
    elif [ "$mgr" = "rpm-ostree" ]; then
        echo "Detected an rpm-ostree system (e.g. Bazzite/Silverblue)." >&2
        echo "Run this script inside a toolbox/distrobox container, or install deps on the host via rpm-ostree." >&2
        echo "Example (host): rpm-ostree install SDL2-devel mesa-libGL-devel libX11-devel libXi-devel libepoxy-devel libcurl-devel openssl-devel" >&2
        exit 1
    else
        echo "Error: unsupported system (no apt-get/dnf/rpm-ostree found)." >&2
        exit 1
    fi
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

PKG_MGR="$(detect_pkg_manager)"

if [ "$PKG_MGR" = "apt" ]; then
    SDL2_PKG="libsdl2-dev"
    CURL_PKG="libcurl4-openssl-dev"
    OPENSSL_PKG="libssl-dev"
    EPOXY_PKG="libepoxy-dev"
    OPENGL_PKG="libgl1-mesa-dev"
    X11_PKG="libx11-dev"
    XI_PKG="libxi-dev"
elif [ "$PKG_MGR" = "dnf" ]; then
    SDL2_PKG="SDL2-devel"
    CURL_PKG="libcurl-devel"
    OPENSSL_PKG="openssl-devel"
    EPOXY_PKG="libepoxy-devel"
    OPENGL_PKG="mesa-libGL-devel"
    X11_PKG="libX11-devel"
    XI_PKG="libXi-devel"
fi

# System deps needed for CMake + linking
for pkg in "$SDL2_PKG" "$CURL_PKG" "$OPENSSL_PKG" "$EPOXY_PKG" "$OPENGL_PKG" "$X11_PKG" "$XI_PKG"; do
    if [ -n "$pkg" ] && ! package_installed "$pkg"; then
        echo "Installing $pkg..."
        install_packages "$pkg"
    fi
done

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

echo ""
echo "Dependencies installed successfully."
echo ""

