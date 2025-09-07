
#define AVC_MODULE_DATA_WRAPPER_NAMESPACE ffmpeg_static
#define AVC_MODULE_DATA_WRAPPER_CLASSNAME AvcModuleProviderDataWrap_Static
#define AVC_MODULE_DATA_WRAPPER_FACTORYNAME AvcModuleProviderDataWrapFactory_Static

#include "loader_common.h"

extern "C" {
namespace AVC_MODULE_DATA_WRAPPER_NAMESPACE {

#include "libavutil/avutil.h"
#include "libavutil/channel_layout.h"
#include "libavdevice/avdevice.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libavutil/hwcontext.h"

struct SwrContext;
#include "libswresample/swresample.h"

}// namespace
}// extern "C"

#include "avc_module_data_wrapper.hpp"
