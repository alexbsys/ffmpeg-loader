# FFmpeg Dynamic Loading Wrapper Library

This library solves fundamental FFmpeg compatibility problems, providing developers with a stable and reliable media processing interface without versioning headaches.


## Overview

A universal C++ wrapper library that enables dynamic loading and interaction with various FFmpeg versions (starting from 3.2 to 8.0): `libavcodec`, `libavutil`, `libavdevice`, `libavformat`, `libswresample`, and `libswscale`. 
Allows client applications to work with any FFmpeg version without recompilation through a unified stable interface.
Built on Dependency Injection principles, it provides an abstraction layer over native FFmpeg APIs, solving binary compatibility issues between different library versions.

## Supported Platforms

Windows, Linux, macOS, Android, iOS, OpenWRT.


## Compatibility Challenge

Native FFmpeg integration faces fundamental compatibility issues:

1. **Binary Data Structure Incompatibility**

FFmpeg data structures (AVCodecContext, AVFrame, etc.) are unstable between versions — field layout, sizes, and composition change constantly. Even with dynamic function loading, direct structure access causes crashes due to memory layout mismatches.

2. **Pixel Format Incompatibility**

The pixel format system (AVPixelFormat) varies even within the same version:

Different format codes for hardware accelerators (VDPAU, CUDA, VAAPI): dynamic numbering changes when built with specific hardware support

3. **Versioning Complexity**
Each FFmpeg component has its own versioning system:

Major and minor versions for each libav* component, fields in the data structures appearing and disappearing within the same major version


## Static Linking Limitations

Static linking of FFmpeg libraries approach creates new problems:

* Binary Distribution Complexity — particularly in Linux/macOS ecosystems where `libavdevice` tightly integrates with system libraries, and user cannot change installed version of libraries

* Version Conflicts — applications with embedded FFmpeg specific version cannot run everywhere

* Hardware Accelerator Issues — statically built FFmpeg with specific SDK support (NVIDIA, Intel Media SDK) often fails on other systems

* No Fallback Mechanisms — inability to handle errors when FFmpeg cannot be loaded


## Solution Architecture

Dynamic Loading with Abstraction
The library implements a multi-layer abstraction system:

* Versioned Namespaces — each FFmpeg data structure set is encapsulated in separate C++ namespaces (ffmpeg_3_2, ffmpeg_4_0, ffmpeg_7_1 etc.)

* Dynamic Function Resolution — all FFmpeg functions are loaded via dlsym/GetProcAddress at runtime

* Unified Access Interface — developers work through a stable C++ API independent of FFmpeg version

* Automatic Version Detection — the library automatically detects loaded FFmpeg version and selects the appropriate implementation


## Key Features

* Zero Overhead Abstraction — minimal performance impact through template-based design

* Type-Safe Interface — strong typing via C++ wrappers

* Exception Safety — guaranteed safety during loading errors

* Cross-Platform — single codebase for all supported platforms

* Extensible — easy addition of new FFmpeg version support


## Benefits

* Guaranteed Compatibility — applications work with any FFmpeg version ≥ 3.2

* Simplified Distribution — single binary build for all systems

* Automatic Fallback — runtime switching between versions

* Future-Proof — new FFmpeg version support added without client code changes



## How to build

`ffmpeg-loader` can be compiled as shared or as static library (default), which can be used inside your application.
Also it can load FFmpeg libraries dynamically, or statically (useful for **ios**).


Simple build (loader builds as static library, FFmpeg libraries are loaded in runtime)
```
cmake -B build
```


Set up loader for loader dynamic build. FFmpeg loads statically, user directories with FFmpeg are provided in command line:
```
cmake -DFFMPEGLOADER_LOAD_AVC_STATICALLY=ON -DFFMPEGLOADER_FFMPEG_INCLUDE_DIR="n:\ffmpeg\include\other\" -DFFMPEGLOADER_FFMPEG_LIB_DIR="n:\ffmpeg\lib\win_x86_64\" -B build
```
