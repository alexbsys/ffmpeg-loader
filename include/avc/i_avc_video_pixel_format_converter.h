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

#ifndef I_AVC_VIDEO_PIXEL_FORMAT_CONVERTER_HEADER
#define I_AVC_VIDEO_PIXEL_FORMAT_CONVERTER_HEADER

#include <media/video_pixel_format.h>

namespace avc {
	
struct IAvcVideoPixelFormatConverter {
  virtual ~IAvcVideoPixelFormatConverter() = default;

  virtual int VideoPixelFormatToAVPixelFormat(enum cmf::VideoPixelFormat video_pixel_format) const = 0;
  virtual enum cmf::VideoPixelFormat AVPixelFormatToVideoPixelFormat(int av_pixel_format) const = 0;
};

}//namespace avc

#endif //I_AVC_VIDEO_PIXEL_FORMAT_CONVERTER_HEADER
