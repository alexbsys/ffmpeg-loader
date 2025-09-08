
#ifndef DEBUG_PRINT
#define DEBUG_PRINT 1
#endif //DEBUG_PRINT

#include "avc_pixel_format_converter.h"
#include <tools/dynamic_export.h>
#include <string>

#if DEBUG_PRINT
#include <cstdio>
#endif //DEBUG_PRINT

using namespace cmf;

namespace avc {

struct TranslatePixelTableElement {
  const char* name_;
  cmf::VideoPixelFormat pix_fmt_;
};

static const TranslatePixelTableElement g_pixfmt_translation_table[] = {
  { "yuv420p", VideoPixelFormat_YUV420P },
  { "yuv422p", VideoPixelFormat_YUV422P },
  { "yuv410p", VideoPixelFormat_YUV410P },
  { "yuv411p", VideoPixelFormat_YUV411P },
  { "yuv440p", VideoPixelFormat_YUV440P },
  { "yuv444p", VideoPixelFormat_YUV444P },
  { "nv12", VideoPixelFormat_NV12 },
  { "nv21", VideoPixelFormat_NV21 },

  { "yuva420p", VideoPixelFormat_YUVA420P },

  // interleaved
  { "yuyv422", VideoPixelFormat_YUYV422 },
  { "yvyu422", VideoPixelFormat_YVYU422 },
  { "uyvy422", VideoPixelFormat_UYVY422 },
  { "uyyvyy411", VideoPixelFormat_UYYVYY411 },

  // gray
  { "gray", VideoPixelFormat_GRAY8 },
  { "gray16be", VideoPixelFormat_GRAY16BE },
  { "gray16le", VideoPixelFormat_GRAY16LE },
  { "monow", VideoPixelFormat_MONOWHITE },
  { "monob", VideoPixelFormat_MONOBLACK },

  { "pal8", VideoPixelFormat_PAL8 },

  // jpeg
  { "yuvj420p", VideoPixelFormat_YUVJ420P },
  { "yuvj422p", VideoPixelFormat_YUVJ422P },
  { "yuvj411p", VideoPixelFormat_YUVJ411P },
  { "yuvj440p", VideoPixelFormat_YUVJ440P },
  { "yuvj444p", VideoPixelFormat_YUVJ444P },

  // rgb
  { "rgb24", VideoPixelFormat_RGB24 },
  { "bgr24", VideoPixelFormat_BGR24 },
  { "bgr8", VideoPixelFormat_BGR8 },
  { "bgr4", VideoPixelFormat_BGR4 },
  { "bgr4_byte", VideoPixelFormat_BGR4_BYTE },
  { "rgb8", VideoPixelFormat_RGB8 },
  { "rgb4", VideoPixelFormat_RGB4 },
  { "rgb4_byte", VideoPixelFormat_RGB4_BYTE },

  { "argb", VideoPixelFormat_ARGB },
  { "rgba", VideoPixelFormat_RGBA },
  { "abgr", VideoPixelFormat_ABGR },
  { "bgra", VideoPixelFormat_BGRA },

  { "rgb565be", VideoPixelFormat_RGB565BE },
  { "rgb565le", VideoPixelFormat_RGB565LE },
  { "rgb555be", VideoPixelFormat_RGB555BE },
  { "rgb555le", VideoPixelFormat_RGB555LE },
  { "bgr565be", VideoPixelFormat_BGR565BE },
  { "bgr565le", VideoPixelFormat_BGR565LE },
  { "bgr555be", VideoPixelFormat_BGR555BE },
  { "bgr555le", VideoPixelFormat_BGR555LE },

  { "rgb48be", VideoPixelFormat_RGB48BE },
  { "rgb48le", VideoPixelFormat_RGB48LE },
  { "bgr48be", VideoPixelFormat_BGR48BE },
  { "bgr48le", VideoPixelFormat_BGR48LE },

  { nullptr, VideoPixelFormat_NONE }
};

std::shared_ptr<IAvcVideoPixelFormatConverter> API_EXPORT CreateAvcPixelFormatConverter(IAvcModuleProvider *avc_module_provider) {
  return std::make_shared<avc::detail::AvcVideoPixelFormatConverter>(avc_module_provider);
}

namespace detail {

AvcVideoPixelFormatConverter::AvcVideoPixelFormatConverter(IAvcModuleProvider* avc_module_provider) {
  InitTables(avc_module_provider);
}

int AvcVideoPixelFormatConverter::VideoPixelFormatToAVPixelFormat(enum cmf::VideoPixelFormat video_pixel_format) const {
  auto it = cmf_to_avc_.find(static_cast<int>(video_pixel_format));
  if (it == cmf_to_avc_.end())
    return -1 /*AV_PIX_FMT_NONE*/;

  return it->second;
}

enum cmf::VideoPixelFormat AvcVideoPixelFormatConverter::AVPixelFormatToVideoPixelFormat(int av_pixel_format) const {
  auto it = avc_to_cmf_.find(av_pixel_format);
  if (it == avc_to_cmf_.end())
    return VideoPixelFormat_NONE;

  return static_cast<enum cmf::VideoPixelFormat>(it->second);
}


void AvcVideoPixelFormatConverter::InitTables(IAvcModuleProvider *avc_module_provider) {
  static const int kAvcMaxPixelFormat = 128;

  if (!avc_module_provider)
    return;

  for (int i=0; i<kAvcMaxPixelFormat; i++) {
    const AVPixFmtDescriptor* fmt = avc_module_provider->av_pix_fmt_desc_get(i);
    if (!fmt)
      continue;

    for (int j=0; ; j++) {
      const auto tr = g_pixfmt_translation_table[j];
      if (tr.name_ == nullptr)
        break;

      if (std::string(tr.name_) != avc_module_provider->d()->AVPixFmtDescriptorGetName(fmt)) {
        continue;
      }

      avc_to_cmf_.insert(std::make_pair(i, static_cast<int>(tr.pix_fmt_)));
      if (cmf_to_avc_.find(tr.pix_fmt_) == cmf_to_avc_.end()) {
        cmf_to_avc_.insert(std::make_pair(static_cast<int>(tr.pix_fmt_), i));
      } else {
#if DEBUG_PRINT
        printf("WARNING: cmf pixel format %d already added\n", tr.pix_fmt_);
#endif //DEBUG_PRINT
      }
    }
  }
}

}//namespace detail
}//namespace avc
