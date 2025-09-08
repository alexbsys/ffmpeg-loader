
#ifndef AVC_PIXEL_FORMAT_CONVERTER_HEADER
#define AVC_PIXEL_FORMAT_CONVERTER_HEADER

#include <media/video_pixel_format.h>
#include <avc/i_avc_module_provider.h>
#include <avc/i_avc_video_pixel_format_converter.h>
#include <map>

namespace avc {

std::shared_ptr<IAvcVideoPixelFormatConverter> CreateAvcPixelFormatConverter(IAvcModuleProvider* avc_module_provider);

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
