#!/bin/bash

# Clean
rm -rf 3rdparty/SDL2
rm -rf 3rdparty/imgui
rm -rf 3rdparty/imgui-node-editor

# SDL2
# You will have to do this yourself
# For CentOS: dnf --enablerepo=powertools install SDL2-devel

# IMGUI
git clone --depth=1 https://github.com/ocornut/imgui 3rdparty/imgui

# ImNodes
git clone --depth=1 https://github.com/Nelarius/imnodes 3rdparty/imnodes

