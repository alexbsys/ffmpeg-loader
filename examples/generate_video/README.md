
# Generate video example

`generate_video__regular_ffmpeg.cc.example` contains source code for regular FFmpeg linking.

`generate_video_ffmpeg_loader.cc` contains adapted source code with ffmpeg loader usage.

## How to test

1. Build executable

2. Run it without FFmpeg binaries. Error must be reported

3. Copy any version of FFmpeg binaries (DLL, so, dylib) to directory with executable and run app. Video file must be generated.

4. Try to replace FFmpeg binaries with different version without recompilation app. Everything should work

