
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
