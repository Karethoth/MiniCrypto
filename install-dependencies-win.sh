#!/bin/bash

# Clean
rm -rf 3rdparty/SDL2
rm -rf 3rdparty/imgui
rm -rf 3rdparty/imnodes

# SDL2
curl http://www.libsdl.org/release/SDL2-devel-2.0.16-VC.zip --output SDL2.zip
unzip SDL2.zip -d 3rdparty/SDL2/
mv 3rdparty/SDL2/SDL2*/* 3rdparty/SDL2/
rm -rf SDL2.zip 3rdparty/SDL2/SDL2-*

# IMGUI
git clone --depth=1 --branch docking https://github.com/ocornut/imgui 3rdparty/imgui

# ImNodes
git clone --depth=1 https://github.com/Nelarius/imnodes 3rdparty/imnodes

# OpenSSL
git clone --depth=1 git://git.openssl.org/openssl.git 3rdparty/openssl
cd 3rdparty/openssl
git config core.autocrlf false
git config core.eol lf
git checkout .

