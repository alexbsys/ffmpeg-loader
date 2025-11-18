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

#include "avc_module_data_wrapper.h"
#include "i_avc_module_data_wrapper_factory.h"

#ifndef AVC_MODULE_LOADER_DEBUG_PRINT
#define AVC_MODULE_LOADER_DEBUG_PRINT 0
#endif //AVC_MODULE_LOADER_DEBUG_PRINT

#include <vector>
#include <typeinfo>

#if AVC_MODULE_LOADER_DEBUG_PRINT
#include <cstdio>
#endif //AVC_MODULE_LOADER_DEBUG_PRINT

#define TOSTR(s) TOSTR_HELPER(s)
#define TOSTR_HELPER(s) #s

#define XXCCAT(s,v) XCCAT(s,v)
#define XCCAT(s,v) s ## v

#define STATIC_CLASS_NAME XXCCAT(AVC_MODULE_DATA_WRAPPER_FACTORYNAME,_Static)
#define STATIC_CLASS_VAR_NAME XXCCAT(AVC_MODULE_DATA_WRAPPER_FACTORYNAME,_StaticVar)
#define CREATE_FACTORY_FN_NAME XXCCAT(AVC_MODULE_DATA_WRAPPER_FACTORYNAME,_Create)

#if defined(_MSC_VER)
#define USE_SAFE_STRING_FUNCTIONS 1
#else //_MSC_VER
#define USE_SAFE_STRING_FUNCTIONS 0
#endif //_MSC_VER

#if USE_SAFE_STRING_FUNCTIONS
#define SAFE_STRCPY(dest, dest_size, src) strcpy_s(dest, dest_size, src)
#else //USE_SAFE_STRING_FUNCTIONS
#define SAFE_STRCPY(dest, dest_size, src) strcpy(dest, src)
#endif //USE_SAFE_STRING_FUNCTIONS

#if defined(_MSC_VER)
#define DISABLE_DEPRECATION_WARNING __pragma(warning(push)) __pragma(warning(disable:4996))
#define RESTORE_DEPRECATION_WARNING __pragma(warning(pop))
#elif defined(__GNUC__) || defined(__clang__)
#define DISABLE_DEPRECATION_WARNING _Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#define RESTORE_DEPRECATION_WARNING _Pragma("GCC diagnostic pop")
#else
#define DISABLE_DEPRECATION_WARNING
#define RESTORE_DEPRECATION_WARNING
#endif

extern std::vector<std::shared_ptr<avc::IAvcModuleDataWrapperFactory> > g_ffmpeg_data_wrappers;

#if !defined(AVC_PREINCLUDED_HEADERS)
extern "C" {

namespace AVC_MODULE_DATA_WRAPPER_NAMESPACE {
#include <libavutil/avutil.h>
#include <libavdevice/avdevice.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>

struct SwrContext;
#include <libswresample/swresample.h>
}// namespace

}// extern "C"
#endif //AVC_PREINCLUDED_HEADERS

namespace avc {
namespace detail {

AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVC_MODULE_DATA_WRAPPER_CLASSNAME(std::shared_ptr<IAvcModuleProvider> module_provider)
  :module_provider_(module_provider)
{
}

AVC_MODULE_DATA_WRAPPER_CLASSNAME::~AVC_MODULE_DATA_WRAPPER_CLASSNAME() {
}

////
// AVStream

AVCodecParameters* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVStreamGetCodecPar(const AVStream* stream) const {
  auto stream_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVStream*>(stream);
  return reinterpret_cast<AVCodecParameters*>(stream_d->codecpar);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVStreamSetTimeBase(AVStream* stream, cmf::MediaTimeBase tb) const {
  auto stream_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVStream*>(stream);
  stream_d->time_base.num = tb.num_;
  stream_d->time_base.den = tb.den_;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVStreamSetFrameRate(AVStream* stream, cmf::MediaTimeBase framerate) const {
  auto stream_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVStream*>(stream);
  stream_d->r_frame_rate.num = framerate.num_;
  stream_d->r_frame_rate.den = framerate.den_;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVStreamSetAvgFrameRate(AVStream* stream, cmf::MediaTimeBase framerate) const {
  auto stream_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVStream*>(stream);
  stream_d->avg_frame_rate.num = framerate.num_;
  stream_d->avg_frame_rate.den = framerate.den_;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVStreamSetStartTime(AVStream* stream, int64_t start_time) const {
  auto stream_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVStream*>(stream);
  stream_d->start_time = start_time;
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVStreamGetStartTime(const AVStream* stream) const {
  auto stream_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVStream*>(stream);
  return stream_d->start_time;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVStreamGetIndex(const AVStream* stream) const {
  auto stream_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVStream*>(stream);
  return stream_d->index;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVStreamSetIndex(AVStream* stream, int index) const {
  auto stream_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVStream*>(stream);
  stream_d->index = index;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVStreamGetId(const AVStream* stream) const {
  auto stream_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVStream*>(stream);
  return stream_d->id;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVStreamSetId(AVStream* stream, int id) const {
  auto stream_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVStream*>(stream);
  stream_d->id = id;
}

cmf::MediaTimeBase AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVStreamGetTimeBase(const AVStream* stream) const {
  auto stream_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVStream*>(stream);
  return cmf::MediaTimeBase(stream_d->time_base.num, stream_d->time_base.den);
}

cmf::MediaTimeBase AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVStreamGetFrameRate(const AVStream* stream) const {
  auto stream_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVStream*>(stream);
  return cmf::MediaTimeBase(stream_d->r_frame_rate.num, stream_d->r_frame_rate.den);
}

cmf::MediaTimeBase AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVStreamGetAvgFrameRage(const AVStream* stream) const {
  auto stream_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVStream*>(stream);
  return cmf::MediaTimeBase(stream_d->avg_frame_rate.num, stream_d->avg_frame_rate.den);
}

////
// AVFormatContext

AVStream* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextGetStreamByIdx(const AVFormatContext* ctx, int idx) const {
  auto ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  return reinterpret_cast<AVStream*>(ctx_d->streams[idx]);
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextGetAvoidNegativeTs(const AVFormatContext* ctx) const {
  auto ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  return ctx_d->avoid_negative_ts;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextGetNbStreams(const AVFormatContext* ctx) const {
  auto ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  return ctx_d->nb_streams;
}

AVIOContext* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextGetPb(const AVFormatContext* ctx) const {
  auto ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  return reinterpret_cast<AVIOContext*>(ctx_d->pb);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextSetPb(AVFormatContext* ctx, AVIOContext* avio_ctx) const {
  auto ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  ctx_d->pb = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVIOContext*>(avio_ctx);
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextGetFlags(const AVFormatContext* ctx) const {
  auto ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  return ctx_d->flags;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextSetFlags(AVFormatContext* ctx, int flags) const {
  auto ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  ctx_d->flags = flags;
}

unsigned char* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVIOContextGetBuffer(const AVIOContext* ctx) const {
  auto ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVIOContext*>(ctx);
  return ctx_d->buffer;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVIOContextSetBuffer(AVIOContext* ctx, unsigned char* buffer) const {
  auto ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVIOContext*>(ctx);
  ctx_d->buffer = buffer;
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextGetStartTime(const AVFormatContext* ctx) const {
  auto ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  return ctx_d->start_time;
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextGetDuration(const AVFormatContext* ctx) const {
  auto ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  return ctx_d->duration;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextSetProbeSize(AVFormatContext* ctx, int64_t probesize) const {
  auto ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  ctx_d->probesize = probesize;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextSetMaxAnalyzeDuration(AVFormatContext* ctx, int64_t max_analyze_duration) const {
  auto ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  ctx_d->max_analyze_duration = max_analyze_duration;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextSetInterruptCallback(AVFormatContext* ctx, int (*callback)(void*), void *opaque) const {
  auto ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  ctx_d->interrupt_callback.callback = callback;
  ctx_d->interrupt_callback.opaque = opaque;
}

const AVInputFormat* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextGetInputFormat(const AVFormatContext* ctx) const {
  auto ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  return reinterpret_cast<const AVInputFormat*>(ctx_d->iformat);
}

const AVOutputFormat* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextGetOutputFormat(const AVFormatContext* ctx) const {
  auto ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  return reinterpret_cast<const AVOutputFormat*>(ctx_d->oformat);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextSetUrl(AVFormatContext* ctx, char* url) const {
  auto ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
#if LIBAVFORMAT_VERSION_MAJOR < 58
  SAFE_STRCPY(ctx_d->filename, sizeof(ctx_d->filename), url);
  module_provider_.lock()->av_free(url);
#else
  ctx_d->url = url;
#endif
}

char* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextGetUrl(const AVFormatContext* ctx) const {
  auto ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
#if LIBAVFORMAT_VERSION_MAJOR < 58
  return (char*)ctx_d->filename;
#else
  return ctx_d->url;
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextSetStartTime(AVFormatContext* ctx, int64_t start_time) const {
  auto ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  ctx_d->start_time = start_time;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFormatContextSetAvoidNegativeTs(AVFormatContext* ctx, int avoid_negative_ts) const {
  auto ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFormatContext*>(ctx);
  ctx_d->avoid_negative_ts = avoid_negative_ts;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVInputFormatGetFlags(const AVInputFormat* iformat) const {
  auto iformat_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVInputFormat*>(iformat);
  return iformat_d->flags;
}

////
// AVOutputFormat

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVOutputFormatGetFlags(const AVOutputFormat* oformat) const {
  auto oformat_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVOutputFormat*>(oformat);
  return oformat_d->flags;
}


////
// AVPacket data structure interaction
int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketGetPts(const AVPacket* pkt) const {
  auto pkt_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  return pkt_d->pts;
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketGetDts(const AVPacket* pkt) const {
  auto pkt_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  return pkt_d->dts;
}

void* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketGetData(const AVPacket* pkt) const {
  auto pkt_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  return pkt_d->data;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketGetSize(const AVPacket* pkt) const {
  auto pkt_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  return pkt_d->size;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketGetStreamIndex(const AVPacket* pkt) const {
  auto pkt_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  return pkt_d->stream_index;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketGetFlags(const AVPacket* pkt) const {
  auto pkt_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  return pkt_d->flags;
}

AVBufferRef* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketGetBuf(const AVPacket* pkt) const {
  auto pkt_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  return reinterpret_cast<AVBufferRef*>(pkt_d->buf);
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketGetPos(const AVPacket* pkt) const {
  auto pkt_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  return pkt_d->pos;
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketGetDuration(const AVPacket* pkt) const {
  auto pkt_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  return pkt_d->duration;
}

cmf::MediaTimeBase AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketGetTimeBase(AVPacket* pkt) const {
  auto pkt_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
#if LIBAVUTIL_VERSION_MAJOR >= 57
  return cmf::MediaTimeBase(pkt_d->time_base.num, pkt_d->time_base.den);
#else 
  return cmf::MediaTimeBase{ 0,0 };
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketSetPts(AVPacket* pkt, int64_t pts) const {
  auto pkt_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  pkt_d->pts = pts;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketSetDts(AVPacket* pkt, int64_t dts) const {
  auto pkt_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  pkt_d->dts = dts;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketSetData(AVPacket* pkt, void* data) const {
  auto pkt_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  pkt_d->data = reinterpret_cast<uint8_t*>(data);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketSetSize(AVPacket* pkt, int size) const {
  auto pkt_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  pkt_d->size = size;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketSetStreamIndex(AVPacket* pkt, int stream_idx) const {
  auto pkt_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  pkt_d->stream_index = stream_idx;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketSetFlags(AVPacket* pkt, int flags) const {
  auto pkt_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  pkt_d->flags = flags;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketSetBuf(AVPacket* pkt, AVBufferRef* buf) const {
  auto pkt_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  pkt_d->buf = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVBufferRef*>(buf);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketSetPos(AVPacket* pkt, int64_t pos) const {
  auto pkt_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  pkt_d->pos = pos;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketSetDuration(AVPacket* pkt, int64_t duration) const {
  auto pkt_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
  pkt_d->duration = duration;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPacketSetTimeBase(AVPacket* pkt, cmf::MediaTimeBase tb) const {
  auto pkt_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPacket*>(pkt);
#if LIBAVUTIL_VERSION_MAJOR >= 57
  pkt_d->time_base.num = tb.num_;
  pkt_d->time_base.den = tb.den_;
#endif //LIBAVUTIL_VERSION_MAJOR
}


/////
// AVCodecContext

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetChannels(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);

#if (LIBAVCODEC_VERSION_MAJOR < 61) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  return codec_context_d->channels;
  RESTORE_DEPRECATION_WARNING
#else 
  return 0;
#endif
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetSampleFormat(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->sample_fmt;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetSampleRate(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->sample_rate;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetWidth(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->width;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetHeight(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->height;
}

const AVCodec* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetCodec(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return reinterpret_cast<const AVCodec*>(codec_context_d->codec);
}

cmf::MediaTimeBase AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetFramerate(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return cmf::MediaTimeBase(codec_context_d->framerate.num, codec_context_d->framerate.den);
}

cmf::MediaTimeBase AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetTimeBase(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return cmf::MediaTimeBase(codec_context_d->time_base.num, codec_context_d->time_base.den);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetTimeBase(AVCodecContext* codec_context, cmf::MediaTimeBase timebase) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->time_base.num = timebase.num_;
  codec_context_d->time_base.den = timebase.den_;
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetBitRate(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->bit_rate;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetBitRate(AVCodecContext* codec_context, int64_t bitrate) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->bit_rate = bitrate;
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetRcMaxRate(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->rc_max_rate;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetRcMaxRate(AVCodecContext* codec_context, int64_t rc_max_rate) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->rc_max_rate = rc_max_rate;
}

cmf::MediaTimeBase AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetPktTimeBase(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return cmf::MediaTimeBase(codec_context_d->pkt_timebase.num, codec_context_d->pkt_timebase.den);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetPktTimeBase(AVCodecContext* codec_context, cmf::MediaTimeBase pkt_time_base) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->pkt_timebase.num = pkt_time_base.num_;
  codec_context_d->pkt_timebase.den = pkt_time_base.den_;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetFrameRate(AVCodecContext* codec_context, cmf::MediaTimeBase frame_rate) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->framerate.num = frame_rate.num_;
  codec_context_d->framerate.den = frame_rate.den_;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetDelay(AVCodecContext* codec_context, int delay) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->delay = delay;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetStrictStdCompliance(AVCodecContext* codec_context, int value) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->strict_std_compliance = value;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetPixFmt(AVCodecContext* codec_context, int pixfmt) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->pix_fmt = static_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPixelFormat>(pixfmt);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetWidth(AVCodecContext* codec_context, int width) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->width = width;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetHeight(AVCodecContext* codec_context, int height) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->height = height;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetProfile(AVCodecContext* codec_context, int profile) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->profile = profile;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetFlags(AVCodecContext* codec_context, int flags) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->flags = flags;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetFlags2(AVCodecContext* codec_context, int flags2) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->flags2 = flags2;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetOpaque(AVCodecContext* codec_context, void* opaque) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->opaque = opaque;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetHwFramesCtx(AVCodecContext* codec_context, AVBufferRef* hw_frames_ctx_buf) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->hw_frames_ctx = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVBufferRef*>(hw_frames_ctx_buf);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetHwDeviceCtx(AVCodecContext* codec_context, AVBufferRef* hw_device_ctx_buf) const {
#if (LIBAVCODEC_VERSION_MAJOR > 57) || (LIBAVCODEC_VERSION_MAJOR == 57 && LIBAVCODEC_VERSION_MINOR >= 89)
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->hw_device_ctx = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVBufferRef*>(hw_device_ctx_buf);
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetSwPixFmt(AVCodecContext* codec_context, int sw_pix_fmt) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->sw_pix_fmt = static_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPixelFormat>(sw_pix_fmt);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetQCompress(AVCodecContext* codec_context, float qcompress) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->qcompress = qcompress;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetFrameSize(AVCodecContext* codec_context, int frame_size) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->frame_size = frame_size;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetPixFmt(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->pix_fmt;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetCodecId(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->codec_id;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetFrameSize(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->frame_size;
}

AVBufferRef* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetHwFramesCtx(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return reinterpret_cast<AVBufferRef*>(codec_context_d->hw_frames_ctx);
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetFlags(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->flags;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetFlags2(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->flags2;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetGopSize(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->gop_size;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetKeyintMin(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->keyint_min;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetMaxBFrames(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->max_b_frames;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetRcBufferSize(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->rc_buffer_size;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetMeCmp(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->me_cmp;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetMeMethod(const AVCodecContext* codec_context) const {
#if FF_API_MOTION_EST && ((LIBAVCODEC_VERSION_MAJOR < 57) || (LIBAVCODEC_VERSION_MAJOR == 57 && LIBAVCODEC_VERSION_MINOR <= 107))
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  DISABLE_DEPRECATION_WARNING
  return codec_context_d->me_method;
  RESTORE_DEPRECATION_WARNING
#else
  return 0;
#endif
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetQMax(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->qmax;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetQMin(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->qmin;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetMaxQDiff(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->max_qdiff;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetMeSubpelQuality(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->me_subpel_quality;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetMeRange(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->me_range;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetThreadType(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->thread_type;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetThreadCount(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->thread_count;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetSlices(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->slices;
}

float AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetIQuantFactor(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->i_quant_factor;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetRefs(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->refs;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetTrellis(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->trellis;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetSampleFmt(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->sample_fmt;
}

uint64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetChannelLayout(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
#if (LIBAVCODEC_VERSION_MAJOR < 61) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  return codec_context_d->channel_layout;
  RESTORE_DEPRECATION_WARNING
#else 
  return 0;
#endif 
}

void* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetOpaque(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->opaque;
}

AVBufferRef* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetHwDeviceCtx(const AVCodecContext* codec_context) const {
#if (LIBAVCODEC_VERSION_MAJOR > 57) || (LIBAVCODEC_VERSION_MAJOR == 57 && LIBAVCODEC_VERSION_MINOR >= 89)
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return reinterpret_cast<AVBufferRef*>(codec_context_d->hw_device_ctx);
#else
  return nullptr;
#endif
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetSwPixFmt(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->sw_pix_fmt;
}

float AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetQCompress(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->qcompress;
}

void* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetPrivData(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->priv_data;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetDelay(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->delay;

}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextGetStrictStdCompliance(const AVCodecContext* codec_context) const {
  auto codec_context_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  return codec_context_d->strict_std_compliance;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetGopSize(AVCodecContext* codec_context, int gop_size) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->gop_size = gop_size;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetKeyintMin(AVCodecContext* codec_context, int keyint_min) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->keyint_min = keyint_min;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetMaxBFrames(AVCodecContext* codec_context, int max_b_frames) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->max_b_frames = max_b_frames;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetRcBufferSize(AVCodecContext* codec_context, int rc_buffer_size) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->rc_buffer_size = rc_buffer_size;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetMeCmp(AVCodecContext* codec_context, int me_cmp) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->me_cmp = me_cmp;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetMeMethod(AVCodecContext* codec_context, int me_method) const {
#if FF_API_MOTION_EST && ((LIBAVCODEC_VERSION_MAJOR < 57) || (LIBAVCODEC_VERSION_MAJOR == 57 && LIBAVCODEC_VERSION_MINOR <= 107))
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  DISABLE_DEPRECATION_WARNING
  codec_context_d->me_method = me_method;
  RESTORE_DEPRECATION_WARNING
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetQMax(AVCodecContext* codec_context, int qmax) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->qmax = qmax;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetQMin(AVCodecContext* codec_context, int qmin) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->qmin = qmin;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetMaxQDiff(AVCodecContext* codec_context, int max_qdiff) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->max_qdiff = max_qdiff;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetMeSubpelQuality(AVCodecContext* codec_context, int me_subpel_quality) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->me_subpel_quality = me_subpel_quality;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetMeRange(AVCodecContext* codec_context, int me_range) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->me_range = me_range;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetThreadType(AVCodecContext* codec_context, int thread_type) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->thread_type = thread_type;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetThreadCount(AVCodecContext* codec_context, int thread_count) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->thread_count = thread_count;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetSlices(AVCodecContext* codec_context, int slices) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->slices = slices;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetIQuantFactor(AVCodecContext* codec_context, float iquant_factor) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->i_quant_factor = iquant_factor;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetRefs(AVCodecContext* codec_context, int refs) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->refs = refs;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetTrellis(AVCodecContext* codec_context, int trellis) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->trellis = trellis;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetSampleFmt(AVCodecContext* codec_context, int sample_fmt) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->sample_fmt = static_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVSampleFormat>(sample_fmt);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetChannelLayout(AVCodecContext* codec_context, uint64_t channel_layout) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
#if (LIBAVCODEC_VERSION_MAJOR < 61) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  codec_context_d->channel_layout = channel_layout;
  RESTORE_DEPRECATION_WARNING
#endif 
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetRcMaxAvailableVbvUse(AVCodecContext* codec_context, float max_available_vbv_use) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->rc_max_available_vbv_use = max_available_vbv_use;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetSampleRate(AVCodecContext* codec_context, int sample_rate) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
  codec_context_d->sample_rate = sample_rate;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecContextSetChannels(AVCodecContext* codec_context, int channels) const {
  auto codec_context_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecContext*>(codec_context);
#if (LIBAVCODEC_VERSION_MAJOR < 61) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  codec_context_d->channels = channels;
  RESTORE_DEPRECATION_WARNING
#endif //LIBAVCODEC_VERSION_MAJOR
}

////////
// AVCodec

const char* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecGetName(const AVCodec* codec) const {
  auto codec_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodec*>(codec);
  return codec_d->name;
}

const char* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecGetLongName(const AVCodec* codec) const {
  auto codec_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodec*>(codec);
  return codec_d->long_name;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecGetCapabilities(const AVCodec* codec) const {
  auto codec_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodec*>(codec);
  return codec_d->capabilities;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecGetId(const AVCodec* codec) const {
  auto codec_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodec*>(codec);
  return codec_d->id;
}

const int* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecGetPixFmts(const AVCodec* codec) const {
  auto codec_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodec*>(codec);

  DISABLE_DEPRECATION_WARNING
  return reinterpret_cast<const int*>(codec_d->pix_fmts);
  RESTORE_DEPRECATION_WARNING
}

const int* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecGetSampleFmts(const AVCodec* codec) const {
  auto codec_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodec*>(codec);

  DISABLE_DEPRECATION_WARNING
  return reinterpret_cast<const int*>(codec_d->sample_fmts);
  RESTORE_DEPRECATION_WARNING
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecGetSupportedFrameRates(const AVCodec* codec, std::vector<cmf::MediaTimeBase>& out_framerates) const {
  auto codec_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodec*>(codec);

  DISABLE_DEPRECATION_WARNING
  const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVRational* frame_rate = codec_d->supported_framerates;
  RESTORE_DEPRECATION_WARNING

  out_framerates.clear();

  while (frame_rate) {
    if (frame_rate->num == 0 && frame_rate->den == 0) break;
    out_framerates.push_back(cmf::MediaTimeBase(frame_rate->num, frame_rate->den));
    frame_rate++;
  }
}

const int* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecGetSupportedSampleRates(const AVCodec* codec) const {
  auto codec_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodec*>(codec);

  DISABLE_DEPRECATION_WARNING
  return codec_d->supported_samplerates;
  RESTORE_DEPRECATION_WARNING
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecGetType(const AVCodec* codec) const {
  auto codec_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodec*>(codec);
  return static_cast<int>(codec_d->type);
}

////
// AVFrame

size_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSizeof() const  {
  return sizeof(AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame);
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetSampleRate(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->sample_rate;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetWidth(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->width;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetHeight(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->height;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetFormat(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->format;
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetPts(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->pts;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetNbSamples(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->nb_samples;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetKeyFrame(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
#if (LIBAVUTIL_VERSION_MAJOR == 58 && LIBAVUTIL_VERSION_MINOR <= 2) || (LIBAVUTIL_VERSION_MAJOR < 58) // last implemented in 6.0
  return avframe_d->key_frame;
#else 
  return 0;
#endif
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetFlags(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->flags;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetPictType(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->pict_type;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetChannels(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
#if (LIBAVCODEC_VERSION_MAJOR < 61) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  return avframe_d->channels;
  RESTORE_DEPRECATION_WARNING
#else
  return 0;
#endif
}

void* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetOpaque(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->opaque;
}

uint8_t* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetData(const AVFrame* avframe, int idx) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->data[idx];
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetLineSize(const AVFrame* avframe, int idx) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->linesize[idx];
}

uint8_t* const* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetDataPtr(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->data;
}

const int* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetLineSizePtr(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->linesize;
}

uint64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetChannelLayout(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
#if (LIBAVCODEC_VERSION_MAJOR < 61) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  return avframe_d->channel_layout;
  RESTORE_DEPRECATION_WARNING
#else 
  return 0;
#endif
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetPktSize(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
#if (LIBAVUTIL_VERSION_MAJOR == 58 && LIBAVUTIL_VERSION_MINOR <= 2) || (LIBAVUTIL_VERSION_MAJOR < 58) // last implemented in 6.0
  return avframe_d->pkt_size;
#else
  return 0;
#endif 
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetPktPos(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
#if (LIBAVCODEC_VERSION_MAJOR < 61) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  return avframe_d->pkt_pos;
  RESTORE_DEPRECATION_WARNING
#else 
  return 0;
#endif
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetPktDuration(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
#if (LIBAVUTIL_VERSION_MAJOR < 59) || (LIBAVUTIL_VERSION_MAJOR == 59 && LIBAVUTIL_VERSION_MINOR < 8) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  return avframe_d->pkt_duration;
  RESTORE_DEPRECATION_WARNING
#else 
  return 0;
#endif
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetPktDts(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return avframe_d->pkt_dts;
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetPktPts(const AVFrame* avframe) const {
#if LIBAVUTIL_VERSION_MAJOR<57
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  DISABLE_DEPRECATION_WARNING
  return avframe_d->pkt_pts;
  RESTORE_DEPRECATION_WARNING
#else
  return AV_NOPTS_VALUE;
#endif //LIBAVUTIL_VERSION_MAJOR
}

AVBufferRef* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetBuf(const AVFrame* avframe, int idx) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return reinterpret_cast<AVBufferRef*>(avframe_d->buf[idx]);
}

AVBufferRef** AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetExtendedBuf(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return reinterpret_cast<AVBufferRef**>(avframe_d->extended_buf);
}

AVDictionary* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetMetaData(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return reinterpret_cast<AVDictionary*>(avframe_d->metadata);
}

AVFrameSideData** AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetSideData(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return reinterpret_cast<AVFrameSideData**>(avframe_d->side_data);
}

AVBufferRef* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetHwFramesCtx(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return reinterpret_cast<AVBufferRef*>(avframe_d->hw_frames_ctx);
}

uint8_t** AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameGetExtendedData(const AVFrame* avframe) const {
  auto avframe_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  return reinterpret_cast<uint8_t**>(avframe_d->extended_data);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetSampleRate(AVFrame* avframe, int sample_rate) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->sample_rate = sample_rate;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetWidth(AVFrame* avframe, int width) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->width = width;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetHeight(AVFrame* avframe, int height) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->height = height;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetFormat(AVFrame* avframe, int format) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->format = format;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetPts(AVFrame* avframe, int64_t pts) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->pts = pts;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetNbSamples(AVFrame* avframe, int nb_samples) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->nb_samples = nb_samples;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetKeyFrame(AVFrame* avframe, int keyframe) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
#if (LIBAVCODEC_VERSION_MAJOR < 61) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  avframe_d->key_frame = keyframe;
  RESTORE_DEPRECATION_WARNING
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetFlags(AVFrame* avframe, int flags) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->flags = flags;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetPictType(AVFrame* avframe, int pict_type) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->pict_type = static_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPictureType>(pict_type);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetChannels(AVFrame* avframe, int channels) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
#if (LIBAVUTIL_VERSION_MAJOR < 59) || (LIBAVUTIL_VERSION_MAJOR == 59 && LIBAVUTIL_VERSION_MINOR < 8) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  avframe_d->channels = channels;
  RESTORE_DEPRECATION_WARNING
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetOpaque(AVFrame* avframe, void* opaque) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->opaque = opaque;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetData(AVFrame* avframe, int idx, uint8_t* data) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->data[idx] = data;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetPktSize(AVFrame* avframe, int pkt_size) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
#if (LIBAVUTIL_VERSION_MAJOR < 60) || (LIBAVUTIL_VERSION_MAJOR == 60 && LIBAVUTIL_VERSION_MINOR < 8) // last implemented in 7.x
  DISABLE_DEPRECATION_WARNING
  avframe_d->pkt_size = pkt_size;
  RESTORE_DEPRECATION_WARNING
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetPktPos(AVFrame* avframe, int64_t pkt_pos) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
#if (LIBAVUTIL_VERSION_MAJOR < 60) || (LIBAVUTIL_VERSION_MAJOR == 60 && LIBAVUTIL_VERSION_MINOR < 8) // last implemented in 7.x
  DISABLE_DEPRECATION_WARNING
  avframe_d->pkt_pos = pkt_pos;
  RESTORE_DEPRECATION_WARNING
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetPktDuration(AVFrame* avframe, int64_t pkt_duration) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
#if (LIBAVUTIL_VERSION_MAJOR < 59) || (LIBAVUTIL_VERSION_MAJOR == 59 && LIBAVUTIL_VERSION_MINOR < 8) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  avframe_d->pkt_duration = pkt_duration;
  RESTORE_DEPRECATION_WARNING
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetPktDts(AVFrame* avframe, int64_t pkt_dts) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->pkt_dts = pkt_dts;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetPktPts(AVFrame* avframe, int64_t pkt_pts) const {
#if LIBAVUTIL_VERSION_MAJOR<57
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  DISABLE_DEPRECATION_WARNING
  avframe_d->pkt_pts = pkt_pts;
  RESTORE_DEPRECATION_WARNING
#endif //LIBAVUTIL_VERSION_MAJOR
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetLineSize(AVFrame* avframe, int idx, int linesize) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->linesize[idx] = linesize;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetChannelLayout(AVFrame* avframe, uint64_t channel_layout) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
#if (LIBAVUTIL_VERSION_MAJOR < 59) || (LIBAVUTIL_VERSION_MAJOR == 59 && LIBAVUTIL_VERSION_MINOR < 8) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  avframe_d->channel_layout = channel_layout;
  RESTORE_DEPRECATION_WARNING
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetBuf(AVFrame* avframe, int idx, AVBufferRef* buf) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->buf[idx] = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVBufferRef*>(buf);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetExtendedBuf(AVFrame* avframe, AVBufferRef** extended_buf) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->extended_buf = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVBufferRef**>(extended_buf);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetMetaData(AVFrame* avframe, AVDictionary* metadata) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->metadata = (AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVDictionary*)metadata;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetSideData(AVFrame* avframe, AVFrameSideData** side_data) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->side_data = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrameSideData**>(side_data);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetHwFramesCtx(AVFrame* avframe, AVBufferRef* hw_frames_ctx) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->hw_frames_ctx = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVBufferRef*>(hw_frames_ctx);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVFrameSetExtendedData(AVFrame* avframe, uint8_t** extended_data) const {
  auto avframe_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVFrame*>(avframe);
  avframe_d->extended_data = extended_data;
}

////
// AVCodecParameters

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetCodecType(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->codec_type;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetWidth(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->width;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetHeight(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->height;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetFormat(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->format;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetChannels(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
#if (LIBAVCODEC_VERSION_MAJOR < 61) || (LIBAVCODEC_VERSION_MAJOR == 61 && LIBAVCODEC_VERSION_MINOR < 3) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  return codecpar_d->channels;
  RESTORE_DEPRECATION_WARNING
#else 
  return 0;
#endif 
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetSampleRate(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->sample_rate;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetExtraDataSize(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->extradata_size;
}

uint8_t* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetExtraData(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->extradata;
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetBitRate(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->bit_rate;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetProfile(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->profile;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetVideoDelay(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->video_delay;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetCodecId(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->codec_id;
}

uint32_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetCodecTag(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->codec_tag;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetFrameSize(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->frame_size;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetBitsPerCodedSample(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->bits_per_coded_sample;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersGetBitsPerRawSample(const AVCodecParameters* codecpar) const {
  auto codecpar_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  return codecpar_d->bits_per_raw_sample;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetCodecType(AVCodecParameters* codecpar, int codec_type) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->codec_type = static_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVMediaType>(codec_type);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetWidth(AVCodecParameters* codecpar, int width) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->width = width;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetHeight(AVCodecParameters* codecpar, int height) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->height = height;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetFormat(AVCodecParameters* codecpar, int format) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->format = format;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetChannels(AVCodecParameters* codecpar, int channels) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
#if (LIBAVCODEC_VERSION_MAJOR < 61) || (LIBAVCODEC_VERSION_MAJOR == 61 && LIBAVCODEC_VERSION_MINOR < 3) // last implemented in 6.x
  DISABLE_DEPRECATION_WARNING
  codecpar_d->channels = channels;
  RESTORE_DEPRECATION_WARNING
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetSampleRate(AVCodecParameters* codecpar, int sample_rate) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->sample_rate = sample_rate;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetExtraDataSize(AVCodecParameters* codecpar, int size) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->extradata_size = size;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetExtraData(AVCodecParameters* codecpar, uint8_t* extradata) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->extradata = extradata;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetBitRate(AVCodecParameters* codecpar, int64_t bitrate) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->bit_rate = bitrate;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetProfile(AVCodecParameters* codecpar, int profile) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->profile = profile;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetCodecId(AVCodecParameters* codecpar, int codec_id) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->codec_id = static_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecID>(codec_id);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetVideoDelay(AVCodecParameters* codecpar, int video_delay) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->video_delay = video_delay;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetCodecTag(AVCodecParameters* codecpar, uint32_t codec_tag) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->codec_tag = codec_tag;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetFrameSize(AVCodecParameters* codecpar, int frame_size) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->frame_size = frame_size;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetBitsPerCodedSample(AVCodecParameters* codecpar, int bits_per_coded_sample) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->bits_per_coded_sample = bits_per_coded_sample;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCodecParametersSetBitsPerRawSample(AVCodecParameters* codecpar, int bits_per_raw_sample) const {
  auto codecpar_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecParameters*>(codecpar);
  codecpar_d->bits_per_raw_sample = bits_per_raw_sample;
}


////
// AVBuffer

uint8_t* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVBufferRefGetData(const AVBufferRef* bufferref) const {
  auto bufferref_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVBufferRef*>(bufferref);
  return bufferref_d->data;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVBufferRefGetSize(const AVBufferRef* bufferref) const {
  auto bufferref_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVBufferRef*>(bufferref);
  return static_cast<int>(bufferref_d->size);
}

////
// AVHWConfig

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWConfigGetPixFmt(const AVCodecHWConfig* hwconfig) const {
#if LIBAVCODEC_VERSION_MAJOR < 58
  return 0;
#else
  auto hwconfig_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecHWConfig*>(hwconfig);
  return hwconfig_d->pix_fmt;
#endif
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWConfigGetDeviceType(const AVCodecHWConfig* hwconfig) const {
#if LIBAVCODEC_VERSION_MAJOR < 58
  return 0;
#else
  auto hwconfig_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecHWConfig*>(hwconfig);
  return hwconfig_d->device_type;
#endif
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWConfigGetMethods(const AVCodecHWConfig* hwconfig) const {
#if LIBAVCODEC_VERSION_MAJOR < 58
  return 0;
#else
  auto hwconfig_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecHWConfig*>(hwconfig);
  return hwconfig_d->methods;
#endif
}

///////
// AVCPBProperties

size_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCPBPropertiesSizeof() const {
  return sizeof(AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCPBProperties);
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCPBPropertiesGetBufferSize(const AVCPBProperties* props) const {
  auto props_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCPBProperties*>(props);
  return static_cast<int>(props_d->buffer_size);
}

uint64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCPBPropertiesGetVbvDelay(const AVCPBProperties* props) const {
  auto props_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCPBProperties*>(props);
  return props_d->vbv_delay;
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCPBPropertiesGetMaxBitrate(const AVCPBProperties* props) const {
  auto props_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCPBProperties*>(props);
  return props_d->max_bitrate;
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCPBPropertiesGetMinBitrate(const AVCPBProperties* props) const {
  auto props_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCPBProperties*>(props);
  return props_d->min_bitrate;
}

int64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCPBPropertiesGetAvgBitrate(const AVCPBProperties* props) const {
  auto props_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCPBProperties*>(props);
  return props_d->avg_bitrate;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCPBPropertiesSetBufferSize(AVCPBProperties* props, int buffer_size) const {
  auto props_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCPBProperties*>(props);
  props_d->buffer_size = buffer_size;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCPBPropertiesSetVbvDelay(AVCPBProperties* props, uint64_t vbv_delay) const {
  auto props_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCPBProperties*>(props);
  props_d->vbv_delay = vbv_delay;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCPBPropertiesSetMaxBitrate(AVCPBProperties* props, int64_t max_bitrate) const {
  auto props_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCPBProperties*>(props);
  props_d->max_bitrate =
#if FF_API_UNSANITIZED_BITRATES || LIBAVCODEC_VERSION_MAJOR<58 || (LIBAVCODEC_VERSION_MAJOR==58 && LIBAVCODEC_VERSION_MINOR<54)
    static_cast<int>(max_bitrate);
#else
    max_bitrate;
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCPBPropertiesSetMinBitrate(AVCPBProperties* props, int64_t min_bitrate) const {
  auto props_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCPBProperties*>(props);
  props_d->min_bitrate =
#if FF_API_UNSANITIZED_BITRATES || LIBAVCODEC_VERSION_MAJOR<58 || (LIBAVCODEC_VERSION_MAJOR==58 && LIBAVCODEC_VERSION_MINOR<54)
    static_cast<int>(min_bitrate);
#else
    min_bitrate;
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVCPBPropertiesSetAvgBitrate(AVCPBProperties* props, int64_t avg_bitrate) const {
  auto props_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCPBProperties*>(props);
  props_d->avg_bitrate =
#if FF_API_UNSANITIZED_BITRATES || LIBAVCODEC_VERSION_MAJOR<58 || (LIBAVCODEC_VERSION_MAJOR==58 && LIBAVCODEC_VERSION_MINOR<54)
    static_cast<int>(avg_bitrate);
#else
    avg_bitrate;
#endif
}

////
// AVHWFramesContext

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWFramesContextGetFormat(const AVHWFramesContext* hwframes_ctx) const {
  auto hwframes_ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWFramesContext*>(hwframes_ctx);
  return hwframes_ctx_d->format;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWFramesContextGetSwFormat(const AVHWFramesContext* hwframes_ctx) const {
  auto hwframes_ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWFramesContext*>(hwframes_ctx);
  return hwframes_ctx_d->sw_format;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWFramesContextGetWidth(const AVHWFramesContext* hwframes_ctx) const {
  auto hwframes_ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWFramesContext*>(hwframes_ctx);
  return hwframes_ctx_d->width;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWFramesContextGetHeight(const AVHWFramesContext* hwframes_ctx) const {
  auto hwframes_ctx_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWFramesContext*>(hwframes_ctx);
  return hwframes_ctx_d->height;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWFramesContextSetFormat(AVHWFramesContext* hwframes_ctx, int format) const {
  auto hwframes_ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWFramesContext*>(hwframes_ctx);
  hwframes_ctx_d->format = static_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPixelFormat>(format);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWFramesContextSetSwFormat(AVHWFramesContext* hwframes_ctx, int sw_format) const {
  auto hwframes_ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWFramesContext*>(hwframes_ctx);
  hwframes_ctx_d->sw_format = static_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPixelFormat>(sw_format);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWFramesContextSetWidth(AVHWFramesContext* hwframes_ctx, int width) const {
  auto hwframes_ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWFramesContext*>(hwframes_ctx);
  hwframes_ctx_d->width = width;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWFramesContextSetHeight(AVHWFramesContext* hwframes_ctx, int height) const {
  auto hwframes_ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWFramesContext*>(hwframes_ctx);
  hwframes_ctx_d->height = height;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWFramesContextSetInitialPoolSize(AVHWFramesContext* hwframes_ctx, int initial_pool_size) const {
  auto hwframes_ctx_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWFramesContext*>(hwframes_ctx);
  hwframes_ctx_d->initial_pool_size = initial_pool_size;
}

///////////
// AVHWAccel

AVHWAccel* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWAccelNew() {
  auto module_provider = module_provider_.lock();
  if (!module_provider)
    return nullptr;
  AVHWAccel* hwaccel = reinterpret_cast<AVHWAccel*>(module_provider->av_malloc(sizeof(AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWAccel)));
  memset(hwaccel, 0, sizeof(AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWAccel));
  return hwaccel;
}

const char* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWAccelGetName(const AVHWAccel* hwaccel) const {
  auto hwaccel_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWAccel*>(hwaccel);
  return hwaccel_d->name;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWAccelGetId(const AVHWAccel* hwaccel) const {
  auto hwaccel_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWAccel*>(hwaccel);
  return hwaccel_d->id;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWAccelGetType(const AVHWAccel* hwaccel) const {
  auto hwaccel_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWAccel*>(hwaccel);
  return hwaccel_d->type;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWAccelGetPixFmt(const AVHWAccel* hwaccel) const {
  auto hwaccel_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWAccel*>(hwaccel);
  return hwaccel_d->pix_fmt;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWAccelGetCapabilities(const AVHWAccel* hwaccel) const {
  auto hwaccel_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWAccel*>(hwaccel);
  return hwaccel_d->capabilities;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWAccelSetName(AVHWAccel* hwaccel, const char* name) const {
  auto hwaccel_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWAccel*>(hwaccel);
  hwaccel_d->name = name;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWAccelSetId(AVHWAccel* hwaccel, int id) const {
  auto hwaccel_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWAccel*>(hwaccel);
  hwaccel_d->id = static_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVCodecID>(id);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWAccelSetType(AVHWAccel* hwaccel, int type) const {
  auto hwaccel_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWAccel*>(hwaccel);
  hwaccel_d->type = static_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVMediaType>(type);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWAccelSetPixFmt(AVHWAccel* hwaccel, int pix_fmt) const {
  auto hwaccel_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWAccel*>(hwaccel);
  hwaccel_d->pix_fmt = static_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPixelFormat>(pix_fmt);
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVHWAccelSetCapabilities(AVHWAccel* hwaccel, int capabilities) const {
  auto hwaccel_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVHWAccel*>(hwaccel);
  hwaccel_d->capabilities = capabilities;
}

////
// AVPixFmtDescriptor

const char* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPixFmtDescriptorGetName(const AVPixFmtDescriptor* descr) const {
  auto pixdesc_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPixFmtDescriptor*>(descr);
  return pixdesc_d->name;
}

const char* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPixFmtDescriptorGetAlias(const AVPixFmtDescriptor* descr) const {
  auto pixdesc_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPixFmtDescriptor*>(descr);
  return pixdesc_d->alias;
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPixFmtDescriptorGetNbComponents(const AVPixFmtDescriptor* descr) const {
  auto pixdesc_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPixFmtDescriptor*>(descr);
  return pixdesc_d->nb_components;
}

uint64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVPixFmtDescriptorGetFlags(const AVPixFmtDescriptor* descr) const {
  auto pixdesc_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVPixFmtDescriptor*>(descr);
  return pixdesc_d->flags;
}


AVChannelLayout* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVChannelLayoutAllocate() const {
#if (LIBAVUTIL_VERSION_MAJOR > 57) || (LIBAVUTIL_VERSION_MAJOR == 57 && LIBAVUTIL_VERSION_MINOR >= 28) // first implemented in 5.1
  auto provider = module_provider_.lock();
  if (provider) {
    AVChannelLayout* layout = reinterpret_cast<AVChannelLayout*>(provider->av_malloc(sizeof(AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVChannelLayout)));
    return layout;
  }
#endif
  return nullptr;
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVChannelLayoutFree(AVChannelLayout* layout) const {
#if (LIBAVUTIL_VERSION_MAJOR > 57) || (LIBAVUTIL_VERSION_MAJOR == 57 && LIBAVUTIL_VERSION_MINOR >= 28) // first implemented in 5.1
  auto provider = module_provider_.lock();
  if (provider)
    provider->av_free(layout);
#endif
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVChannelLayoutGetOrder(const AVChannelLayout* layout) const {
#if (LIBAVUTIL_VERSION_MAJOR > 57) || (LIBAVUTIL_VERSION_MAJOR == 57 && LIBAVUTIL_VERSION_MINOR >= 28) // first implemented in 5.1
  auto layout_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVChannelLayout*>(layout);
  return layout_d->order;
#else
  return 0;
#endif
}

int AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVChannelLayoutGetNbChannels(const AVChannelLayout* layout) const {
#if (LIBAVUTIL_VERSION_MAJOR > 57) || (LIBAVUTIL_VERSION_MAJOR == 57 && LIBAVUTIL_VERSION_MINOR >= 28) // first implemented in 5.1
  auto layout_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVChannelLayout*>(layout);
  return layout_d->nb_channels;
#else
  return 0;
#endif
}

void* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVChannelLayoutGetOpaque(const AVChannelLayout* layout) const {
#if (LIBAVUTIL_VERSION_MAJOR > 57) || (LIBAVUTIL_VERSION_MAJOR == 57 && LIBAVUTIL_VERSION_MINOR >= 28) // first implemented in 5.1
  auto layout_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVChannelLayout*>(layout);
  return layout_d->opaque;
#else
  return nullptr;
#endif
}

uint64_t AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVChannelLayoutGetMask(const AVChannelLayout* layout) const {
#if (LIBAVUTIL_VERSION_MAJOR > 57) || (LIBAVUTIL_VERSION_MAJOR == 57 && LIBAVUTIL_VERSION_MINOR >= 28) // first implemented in 5.1
  auto layout_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVChannelLayout*>(layout);
  return layout_d->u.mask;
#else
  return 0;
#endif
}

const AVChannelCustom* AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVChannelLayoutGetMap(const AVChannelLayout* layout) const {
#if (LIBAVUTIL_VERSION_MAJOR > 57) || (LIBAVUTIL_VERSION_MAJOR == 57 && LIBAVUTIL_VERSION_MINOR >= 28) // first implemented in 5.1
  auto layout_d = reinterpret_cast<const AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVChannelLayout*>(layout);
  return (const avc::AVChannelCustom*)layout_d->u.map;
#else
  return nullptr;
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVChannelLayoutSetOrder(AVChannelLayout* layout, int order) const {
#if (LIBAVUTIL_VERSION_MAJOR > 57) || (LIBAVUTIL_VERSION_MAJOR == 57 && LIBAVUTIL_VERSION_MINOR >= 28) // first implemented in 5.1
  auto layout_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVChannelLayout*>(layout);
  layout_d->order = static_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVChannelOrder>(order);
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVChannelLayoutSetNbChannels(AVChannelLayout* layout, int nb_channels) const {
#if (LIBAVUTIL_VERSION_MAJOR > 57) || (LIBAVUTIL_VERSION_MAJOR == 57 && LIBAVUTIL_VERSION_MINOR >= 28) // first implemented in 5.1
  auto layout_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVChannelLayout*>(layout);
  layout_d->nb_channels = nb_channels;
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVChannelLayoutSetOpaque(AVChannelLayout* layout, void* opaque) const {
#if (LIBAVUTIL_VERSION_MAJOR > 57) || (LIBAVUTIL_VERSION_MAJOR == 57 && LIBAVUTIL_VERSION_MINOR >= 28) // first implemented in 5.1
  auto layout_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVChannelLayout*>(layout);
  layout_d->opaque = opaque;
#endif
}

void AVC_MODULE_DATA_WRAPPER_CLASSNAME::AVChannelLayoutSetMask(AVChannelLayout* layout, uint64_t mask) const {
#if (LIBAVUTIL_VERSION_MAJOR > 57) || (LIBAVUTIL_VERSION_MAJOR == 57 && LIBAVUTIL_VERSION_MINOR >= 28) // first implemented in 5.1
  auto layout_d = reinterpret_cast<AVC_MODULE_DATA_WRAPPER_NAMESPACE::AVChannelLayout*>(layout);
  layout_d->u.mask = mask;
#endif
}

////
// Factory implementation

AVC_MODULE_DATA_WRAPPER_FACTORYNAME::AVC_MODULE_DATA_WRAPPER_FACTORYNAME() {
#if AVC_MODULE_LOADER_DEBUG_PRINT
  printf("AVCLOADER: Add " TOSTR(AVC_MODULE_DATA_WRAPPER_FACTORYNAME)
   " avcodec %.6X, avdevice %.6X, avutil %.6X\n",
   LIBAVCODEC_VERSION_INT, LIBAVDEVICE_VERSION_INT, LIBAVUTIL_VERSION_INT);
#endif //AVC_MODULE_LOADER_DEBUG_PRINT

  version_.avcodec_version_ = LIBAVCODEC_VERSION_INT;
  version_.avdevice_version_ = LIBAVDEVICE_VERSION_INT;
  version_.avformat_version_ = LIBAVFORMAT_VERSION_INT;
  version_.avutil_version_ = LIBAVUTIL_VERSION_INT;
  version_.swresample_version_ = LIBSWRESAMPLE_VERSION_INT;
  version_.swscale_version_ = LIBSWSCALE_VERSION_INT;
}

const AvcModuleVersion& AVC_MODULE_DATA_WRAPPER_FACTORYNAME::GetVersion() const {
  return version_;
}

std::shared_ptr<IAvcModuleDataWrapper> AVC_MODULE_DATA_WRAPPER_FACTORYNAME::Create(std::shared_ptr<IAvcModuleProvider> module_provider) {
  return std::shared_ptr<IAvcModuleDataWrapper>(new detail::AVC_MODULE_DATA_WRAPPER_CLASSNAME(module_provider));
}

class STATIC_CLASS_NAME {
public:
  STATIC_CLASS_NAME() {
    std::shared_ptr<AVC_MODULE_DATA_WRAPPER_FACTORYNAME> factory(new AVC_MODULE_DATA_WRAPPER_FACTORYNAME());
#if AVC_MODULE_LOADER_DEBUG_PRINT
    printf("AVCLOADER: Static %s\n", TOSTR(AVC_MODULE_DATA_WRAPPER_FACTORYNAME) );
#endif//AVC_MODULE_LOADER_DEBUG_PRINT
    g_ffmpeg_data_wrappers.push_back(factory);
  }
};

volatile static STATIC_CLASS_NAME STATIC_CLASS_VAR_NAME;

}//namespace detail
}//namespace cmf

std::shared_ptr<avc::IAvcModuleDataWrapperFactory> CREATE_FACTORY_FN_NAME() {
  return std::shared_ptr<avc::IAvcModuleDataWrapperFactory>(new avc::detail::AVC_MODULE_DATA_WRAPPER_FACTORYNAME());
}
