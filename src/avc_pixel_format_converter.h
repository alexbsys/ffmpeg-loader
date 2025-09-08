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

#ifndef AVC_PIXEL_FORMAT_CONVERTER_HEADER
#define AVC_PIXEL_FORMAT_CONVERTER_HEADER

#include <media/video_pixel_format.h>
#include <avc/i_avc_module_provider.h>
#include <avc/i_avc_video_pixel_format_converter.h>
#include <map>

namespace avc {

std::shared_ptr<IAvcVideoPixelFormatConverter> CreateAvcPixelFormatConverter(
  IAvcModuleProvider* avc_module_provider);

namespace detail {

class AvcVideoPixelFormatConverter
  : public virtual IAvcVideoPixelFormatConverter {
 public:
  AvcVideoPixelFormatConverter(IAvcModuleProvider* avc_module_provider);
  virtual ~AvcVideoPixelFormatConverter() = default;

  int VideoPixelFormatToAVPixelFormat(enum cmf::VideoPixelFormat video_pixel_format) const override;
  enum cmf::VideoPixelFormat AVPixelFormatToVideoPixelFormat(int av_pixel_format) const override;

 private:
  void InitTables(IAvcModuleProvider* avc_module_provider);

  std::map<int, int> cmf_to_avc_;
  std::map<int, int> avc_to_cmf_;
};


}  // namespace detail
}//namespace avc

#endif  // AVC_PIXEL_FORMAT_CONVERTER_HEADER
