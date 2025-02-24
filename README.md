
<div align="center">
    <a href="https://github.com/deathkiller/jazz2-native"><img src="https://raw.githubusercontent.com/deathkiller/jazz2/master/Docs/Logo.gif" alt="Jazz² Resurrection" title="Jazz² Resurrection"></a>
</div>

<div align="center">
    Open-source <strong>Jazz Jackrabbit 2</strong> reimplementation
</div>

<div align="center">
  <sub>
    Brought to you by <a href="https://github.com/deathkiller">@deathkiller</a>
  </sub>
</div>
<hr/>


## Introduction
Jazz² Resurrection is reimplementation of the game **Jazz Jackrabbit 2** released in 1998. Supports various versions of the game (Shareware Demo, Holiday Hare '98, The Secret Files and Christmas Chronicles). Also, it partially supports some features of JJ2+ extension and MLLE. This repository contains fully rewritten game in C++ with better performance and many improvements. Further information can be found [here](http://deat.tk/jazz2/).

[![Build Status](https://img.shields.io/github/workflow/status/deathkiller/jazz2-native/Linux?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCAyNCAyNCI+PHBhdGggZmlsbD0iI2ZmZmZmZiIgZD0iTTI0IDIuNXYxOUwxOCAyNCAwIDE4LjV2LS41NjFsMTggMS41NDVWMHpNMSAxMy4xMTFMNC4zODUgMTAgMSA2Ljg4OWwxLjQxOC0uODI3TDUuODUzIDguNjUgMTIgM2wzIDEuNDU2djExLjA4OEwxMiAxN2wtNi4xNDctNS42NS0zLjQzNCAyLjU4OXpNNy42NDQgMTBMMTIgMTMuMjgzVjYuNzE3eiI+PC9wYXRoPjwvc3ZnPg==)](https://github.com/deathkiller/jazz2-native/actions)
[![Latest Release](https://img.shields.io/github/v/tag/deathkiller/jazz2?label=release)](https://github.com/deathkiller/jazz2/releases/latest)
[![All Downloads](https://img.shields.io/github/downloads/deathkiller/jazz2/total.svg?color=blueviolet)](https://github.com/deathkiller/jazz2/releases)
[![Code Quality](https://img.shields.io/codacy/grade/64eb3ca12bd04c64bf3f3515744b591a.svg?logo=codacy&logoColor=ffffff)](https://www.codacy.com/app/deathkiller/jazz2-native)
[![License](https://img.shields.io/github/license/deathkiller/jazz2-native.svg)](https://github.com/deathkiller/jazz2-native/blob/master/LICENSE)
[![Discord](https://img.shields.io/discord/355651795390955520.svg?color=839ef7&label=chat&logo=discord&logoColor=ffffff&labelColor=586eb5)](https://discord.gg/Y7SBvkD)


## Preview
<div align="center">
    <img src="https://raw.githubusercontent.com/deathkiller/jazz2/master/Docs/Screen2.gif" alt="Preview">
</div>

<div align="center"><a href="https://www.youtube.com/playlist?list=PLfrN-pyVL7k6n2VJF197F0yVOZq4EPTsP">:tv: Watch gameplay videos</a></div>


## Running the application
### Windows
* Download the game
* Copy original *Jazz Jackrabbit 2* directory to `‹Game›\Source\`
* Run `‹Game›\Jazz2.exe`, `‹Game›\Jazz2_avx2.exe` or `‹Game›\Jazz2_sdl2.exe` application

`‹Game›` *is path to Jazz² Resurrection. Cache is recreated during intro cinematics on the first startup, so it can't be skipped.*

### Linux
* Download the game
* Install dependencies: `sudo apt install libglew2.2 libglfw3 libopenal1 libopenmpt0`
* Copy original *Jazz Jackrabbit 2* directory to `‹Game›/Source/`
* Run `‹Game›/jazz2` or `‹Game›/jazz2_sdl2` application

`‹Game›` *is path to Jazz² Resurrection. Cache is recreated during intro cinematics on the first startup, so it can't be skipped.*

<sup>Alternatively, you can use package for your Linux distribution:</sup><br>
[![ArchLinux](https://img.shields.io/badge/Arch%20Linux-grey?logo=archlinux)](https://aur.archlinux.org/packages/jazz2-git)
[![OpenSUSE](https://img.shields.io/badge/OpenSUSE-grey?logo=opensuse)](https://software.opensuse.org/download.html?project=home%3Amnhauke%3Agames&package=jazz2)

### Web (Emscripten)
* Go to http://deat.tk/jazz2/wasm/
* Import episodes from original *Jazz Jackrabbit 2* directory to unlock additional content

### Android
* Download the game
* Copy `Content` directory to `‹Storage›/Android/data/jazz2.resurrection/files/Content/`
* Copy original *Jazz Jackrabbit 2* directory to `‹Storage›/Android/data/jazz2.resurrection/files/Source/`
* Install `Jazz2.apk` or `Jazz2_x86-64.apk` on the device
* Run the newly installed application

`‹Storage›` *is usually internal storage on your device. The game requires device with **Android 5.0** (or newer) and **OpenGL ES 3.0** support. Cache is recreated during intro cinematics on the first startup, so it can't be skipped.*

### Xbox (Universal Windows Platform)
* Download the game
* Install `Jazz2.cer` certificate (the application is self-signed)
* Install `Jazz2.msixbundle` package
* Run the newly installed application
* Copy original *Jazz Jackrabbit 2* directory to destination shown in the main menu
* Run the application again


## Building the application
### Windows
* Download [build dependencies](https://github.com/deathkiller/jazz2-libraries) to `.\Libs\`
* Open the solution in [Microsoft Visual Studio 2019](https://www.visualstudio.com/) (or newer) and build it
  * CMake is **not** recommended for Windows build

### Linux
* Build dependencies will be downloaded automatically by *CMake*
  * Can be disabled with `NCINE_DOWNLOAD_DEPENDENCIES` option, then download [build dependencies](https://github.com/deathkiller/jazz2-libraries) manually to `./Libs/`
  * In case of build errors, install following packages (or equivalent for your distribution):<br>`libgl1-mesa-dev libglew-dev libglfw3-dev libsdl2-dev libopenal-dev libopenmpt-dev zlib1g-dev`
* Build the solution with *CMake*
  * Run `./BuildLinuxGcc.sh` (GLFW) or `./BuildLinuxGcc_SDL2.sh` (SDL2) to build with GCC compiler
  * Run `./BuildLinuxClang.sh` to build with Clang compiler

### Web (Emscripten)
* Install Emscripten SDK (preferably to `../emsdk/`)
```bash
cd ..
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
```
* Build dependencies will be downloaded automatically by *CMake*
  * Can be disabled with `NCINE_DOWNLOAD_DEPENDENCIES` option, then download [build dependencies](https://github.com/deathkiller/jazz2-libraries) manually to `./Libs/`
* Put required game files to `./Content/` directory – the files must be provided in advance
* Build the solution with *CMake*
  * Run `./BuildEmscripten.sh` to build with Emscripten toolchain

### Android
* Install Android SDK (preferably to `../android-sdk/`)
* Install Android NDK (preferably to `../android-ndk/`)
* Install Gradle (preferably to `../gradle/`)
* Build dependencies will be downloaded automatically by *CMake*
  * Can be disabled with `NCINE_DOWNLOAD_DEPENDENCIES` option, then download [build dependencies](https://github.com/deathkiller/jazz2-libraries) manually to `./Libs/`
* Build the solution with *CMake*
  * Run `./BuildAndroid.sh` or `./BuildAndroid_x86-64.sh` to build *APK* for Android
  * Run `./BuildAndroidSign.sh` to sign built *APKs*
    * Keystore file `Keystore.jks` must exist in repository root

### Xbox (Universal Windows Platform)
* Build dependencies will be downloaded automatically by *CMake*
  * Can be disabled with `NCINE_DOWNLOAD_DEPENDENCIES` option, then download [build dependencies](https://github.com/deathkiller/jazz2-libraries) manually to `.\Libs\`
* Run `.\BuildUwp.bat` to create [Microsoft Visual Studio 2019](https://www.visualstudio.com/) (or newer) solution


## License
This project is licensed under the terms of the [GNU General Public License v3.0](./LICENSE).

This project uses modified [nCine](https://github.com/nCine/nCine) game engine released under the following license:
```
Copyright (c) 2011-2022 Angelo Theodorou

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
```