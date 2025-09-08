//
// Copyright (c) 2025, Alex Bobryshev <alexbobryshev555@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef DEFAULT_LIBRARIES_NAMES_HPP
#define DEFAULT_LIBRARIES_NAMES_HPP

#include <string>

namespace avc {
	
// Default names of DLLs for each platform. They can be overriden by user parameters

#ifdef _WIN32
static const std::string kDefaultAvCodecModuleName("avcodec-58.dll");
static const std::string kDefaultAvFormatModuleName("avformat-58.dll");
static const std::string kDefaultAvUtilModuleName("avutil-56.dll");
static const std::string kDefaultAvDeviceModuleName("avdevice-58.dll");
static const std::string kDefaultSwScaleModuleName("swscale-5.dll");
static const std::string kDefaultSwResampleModuleName("swresample-3.dll");

static const std::string kNoVersionAvCodecModuleName("avcodec.dll");
static const std::string kNoVersionAvFormatModuleName("avformat.dll");
static const std::string kNoVersionAvUtilModuleName("avutil.dll");
static const std::string kNoVersionAvDeviceModuleName("avdevice.dll");
static const std::string kNoVersionSwScaleModuleName("swscale.dll");
static const std::string kNoVersionSwResampleModuleName("swresample.dll");

#else /*_WIN32*/
#ifdef __APPLE__
static const std::string kDefaultAvCodecModuleName("libavcodec.58.dylib");
static const std::string kDefaultAvFormatModuleName("libavformat.58.dylib");
static const std::string kDefaultAvUtilModuleName("libavutil.56.dylib");
static const std::string kDefaultAvDeviceModuleName("libavdevice.58.dylib");
static const std::string kDefaultSwScaleModuleName("libswscale.5.dylib");
static const std::string kDefaultSwResampleModuleName("libswresample.3.dylib");

static const std::string kNoVersionAvCodecModuleName("libavcodec.dylib");
static const std::string kNoVersionAvFormatModuleName("libavformat.dylib");
static const std::string kNoVersionAvUtilModuleName("libavutil.dylib");
static const std::string kNoVersionAvDeviceModuleName("libavdevice.dylib");
static const std::string kNoVersionSwScaleModuleName("libswscale.dylib");
static const std::string kNoVersionSwResampleModuleName("libswresample.dylib");

#else /*__APPLE__*/

// LINUX
static const std::string kDefaultAvCodecModuleName("libavcodec.so.58");
static const std::string kDefaultAvFormatModuleName("libavformat.so.58");
static const std::string kDefaultAvUtilModuleName("libavutil.so.56");
static const std::string kDefaultAvDeviceModuleName("libavdevice.so.58");
static const std::string kDefaultSwScaleModuleName("libswscale.so.5");
static const std::string kDefaultSwResampleModuleName("libswresample.so.3");

static const std::string kNoVersionAvCodecModuleName("libavcodec.so");
static const std::string kNoVersionAvFormatModuleName("libavformat.so");
static const std::string kNoVersionAvUtilModuleName("libavutil.so");
static const std::string kNoVersionAvDeviceModuleName("libavdevice.so");
static const std::string kNoVersionSwScaleModuleName("libswscale.so");
static const std::string kNoVersionSwResampleModuleName("libswresample.so");

#endif /*__APPLE__*/
#endif /*_WIN32*/
	
}//namespace avc

#endif //DEFAULT_LIBRARIES_NAMES_HPP
