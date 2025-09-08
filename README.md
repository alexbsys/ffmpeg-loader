# FFmpeg Dynamic Loading Wrapper Library

This library solves fundamental FFmpeg compatibility problems, providing developers with a stable and reliable media processing interface without versioning headaches.

## Library overview

A universal C++ wrapper library that enables dynamic loading and interaction with functions and data structures from various FFmpeg versions (starting from 3.2 to 8.0): `libavcodec`, `libavutil`, `libavdevice`, `libavformat`, `libswresample`, and `libswscale`. 
Allows client applications to work with any FFmpeg version without recompilation through a unified stable interface.
Built on Dependency Injection principles, it provides an abstraction layer over native FFmpeg APIs, solving binary compatibility issues between different library versions.

### FFmpeg compatibility Challenge

Native FFmpeg integration faces fundamental compatibility issues:

1. **Binary Data Structure Incompatibility**

    FFmpeg data structures (AVCodecContext, AVFrame, etc.) are unstable between versions — field layout, sizes, and composition change constantly. Even with dynamic function loading, direct structure access causes crashes due to memory layout mismatches.

2. **Pixel Format Incompatibility**

    The pixel format system (AVPixelFormat) varies even within the same version, depends on different format codes for hardware accelerators (VDPAU, CUDA, VAAPI): dynamic numbering changes when built with specific hardware support. FFmpeg-loader solves that by using dynamic pixel format number binding (pixel formats deduced by names and saved to conversion table)

3. **Versioning Complexity**
    Each FFmpeg component has its own versioning system, major and minor versions for each libav* component. Fields in the data structures appearing and disappearing within the same major version


### Static Linking Limitations

Static linking of FFmpeg libraries approach creates new problems:

* **Binary Distribution Complexity**, particularly in Linux/macOS ecosystems where `libavdevice` tightly integrates with system libraries, and user cannot change installed version of libraries

* **Version Conflicts**: applications with embedded FFmpeg specific version cannot run everywhere

* **Hardware Acceleration specific build issues**. Statically linked FFmpeg with specific SDK support (NVIDIA, Intel Media SDK) often fails on other systems

* **No Fallback Mechanisms**. Your app just cannot start when FFmpeg cannot be loaded

### Benefits of runtime dynamic linkage

* **Guaranteed Compatibility**. Applications work with any FFmpeg version from 3.2 (all functions from 3.4)

* **Simplified Distribution**. Single binary works with all FFmpeg versions.

* **Automatic Fallback**. Runtime switching between versions

* **Future-Proof**. New FFmpeg version support added without client code changes

### FFmpeg-loader solution architecture

The library implements a multi-layer abstraction system:

* **Internal Versioned Namespaces**: each FFmpeg data structure set is encapsulated in separate C++ namespaces (`ffmpeg_3_2`, `ffmpeg_4_0`, `ffmpeg_7_1` etc.)

* **Dynamic Function Resolution**: all FFmpeg functions are loaded via `dlsym`/`GetProcAddress` at runtime

* Unified Access Interface — developers work through a stable C++ API independent of FFmpeg version

* Automatic Version Detection for data interaction — the library automatically detects loaded FFmpeg version and selects the appropriate implementation


### Key features

* **Zero Overhead Abstraction**. Minimal performance impact through template-based design

* **Exception Safety**. Guaranteed safety during loading errors

* **Cross-Platform**. Single codebase for all supported platforms

* **Extensible**. Easy addition of new FFmpeg version support


## Supported Platforms

Windows, Linux, macOS, Android, iOS, OpenWRT.


## How to build

`ffmpeg-loader` can be compiled as shared or as static library (default), which can be used inside your application.
Also it can load FFmpeg libraries dynamically, or statically (useful for **ios**).


Simple build (loader builds as static library, FFmpeg libraries are loaded in runtime)
```bash
cmake -B build
cd build
make -j
```

Set up loader for loader dynamic build. FFmpeg libraries used statically, user directories with FFmpeg are provided in command line:
```bash
cmake -DFFMPEGLOADER_LOAD_AVC_STATICALLY=ON -DFFMPEGLOADER_FFMPEG_INCLUDE_DIR="n:\ffmpeg\include\other" -DFFMPEGLOADER_FFMPEG_LIB_DIR="n:\ffmpeg\lib\win_x86_64" -B build
```

Build static library instead of shared:
```bash
cmake -DFFMPEGLOADER_BUILD_SHARED=OFF -B build
cd build
make -j
```


## User C++ source code adaptation

For best understanding you cal look into `examples` directory: provided example with regular usage of FFmpeg linking, and with loader library.

Common rules:

1. Remove all includes libav* headers. Instead of them please add:
```cpp
#include <avc/ffmpeg-loader.h>
#include <avc/libav_detached_common.h>  // some useful constants from ffmpeg
```

2. Create loader instance and check AVC libraries loaded before usage
```cpp
std::shared_ptr<avc::IAvcModuleProvider> avc_loader = avc::CreateAvcModuleProvider3();
if (!avc_loader->IsAvCodecLoaded() || !avc_loader->IsAvFormatLoaded()) {
  // Dynamic libraries were not loaded - handle error
}
```

3. All direct calls to AVC libraries replace to calls through loader. And prefix FFmpeg data types with `avc::` namespace
```cpp
// replacement for avformat_network_init()
avc_loader->avformat_network_init();  

// replacement for AVFormatContext* fmt_ctx = nullptr
avc::AVFormatContext* fmt_ctx = nullptr;  

// replacement for avformat_alloc_output_context2(&fmt_ctx, nullptr, nullptr, filename)
avc_loader->avformat_alloc_output_context2(&fmt_ctx, nullptr, nullptr, filename); 

```

So, calls and pointers definition is simple. Just perform same call through object.

4. Data structures access adaptation. This is most complicated thing

Before:
```cpp
AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
codec_ctx->width = width;
codec_ctx->height = height;
codec_ctx->time_base = { 1, fps };
codec_ctx->framerate = { fps, 1 };
...
frame->format = codec_ctx->pix_fmt; // frame is AVFrame*, codec_ctx is AVCodecContext*

```

After:
```cpp
avc::AVCodecContext* codec_ctx = avc_loader->avcodec_alloc_context3(codec);
avc_loader->d()->AVCodecContextSetWidth(codec_ctx, width);
avc_loader->d()->AVCodecContextSetHeight(codec_ctx, height);
avc_loader->d()->AVCodecContextSetTimeBase(codec_ctx, cmf::MediaTimeBase(1, fps));
avc_loader->d()->AVCodecContextSetFrameRate(codec_ctx, cmf::MediaTimeBase(fps, 1));
...
avc_loader->d()->AVFrameSetFormat(frame, avc_loader->d()->AVCodecContextGetPixFmt(codec_ctx));
```

In some cases call produces input data structure modification:
```cpp
// this call modifies fmt_ctx->pb pointer
avio_open2(&fmt_ctx->pb, filename, AVIO_FLAG_WRITE, nullptr, nullptr);
```

Solution: call getter, save pointer to local variable, provide pointer to variable into call, set modified value back:
```cpp
avc::AVIOContext* ioctx = avc_loader->d()->AVFormatContextGetPb(fmt_ctx);
avc_loader->avio_open2(&ioctx, filename, AVIO_FLAG_WRITE, nullptr, nullptr);
avc_loader->d()->AVFormatContextSetPb(fmt_ctx, ioctx);  // set modified AVIOContext to pb
```

