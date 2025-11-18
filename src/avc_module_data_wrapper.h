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

#ifndef AVC_MODULE_DATA_WRAPPER_H
#define AVC_MODULE_DATA_WRAPPER_H

#include <avc/i_avc_module_provider.h>
#include <media/media_timebase.h>
#include "i_avc_module_data_wrapper_factory.h"

#include <memory>
#include <cstdint>

#if !defined(AVC_MODULE_DATA_WRAPPER_CLASSNAME)
#define AVC_MODULE_DATA_WRAPPER_CLASSNAME  AvcModuleDataWrapper
#endif //AVC_MODULE_DATA_WRAPPER_CLASSNAME

#if !defined(AVC_MODULE_DATA_WRAPPER_FACTORYNAME)
#define AVC_MODULE_DATA_WRAPPER_FACTORYNAME  AvcModuleDataWrapperFactory
#endif //AVC_MODULE_DATA_WRAPPER_FACTORYNAME

#if !defined(AVC_MODULE_DATA_WRAPPER_NAMESPACE)
#define AVC_MODULE_DATA_WRAPPER_NAMESPACE  ffmpeg324
#endif //AVC_MODULE_DATA_WRAPPER_NAMESPACE

namespace avc {
namespace detail {

class AVC_MODULE_DATA_WRAPPER_CLASSNAME : public virtual IAvcModuleDataWrapper {
public:
  AVC_MODULE_DATA_WRAPPER_CLASSNAME(std::shared_ptr<IAvcModuleProvider> module_provider);
  virtual ~AVC_MODULE_DATA_WRAPPER_CLASSNAME() override;

  cmf::MediaTimeBase AVStreamGetTimeBase(const AVStream* stream) const override;
  cmf::MediaTimeBase AVStreamGetFrameRate(const AVStream* stream) const override;
  cmf::MediaTimeBase AVStreamGetAvgFrameRage(const AVStream* stream) const override;
  int64_t AVStreamGetStartTime(const AVStream* stream) const override;
  int AVStreamGetIndex(const AVStream* stream) const override;
  int AVStreamGetId(const AVStream* stream) const override;
  AVCodecParameters* AVStreamGetCodecPar(const AVStream* stream) const override;

  void AVStreamSetTimeBase(AVStream* stream, cmf::MediaTimeBase tb) const override;
  void AVStreamSetFrameRate(AVStream* stream, cmf::MediaTimeBase framerate) const override;
  void AVStreamSetAvgFrameRate(AVStream* stream, cmf::MediaTimeBase framerate) const override;
  void AVStreamSetStartTime(AVStream* stream, int64_t start_time) const override;
  void AVStreamSetIndex(AVStream* stream, int index) const override;
  void AVStreamSetId(AVStream* stream, int id) const override;

  unsigned char* AVIOContextGetBuffer(const AVIOContext* ctx) const override;
  void AVIOContextSetBuffer(AVIOContext* ctx, unsigned char* buffer) const override;

  int AVFormatContextGetAvoidNegativeTs(const AVFormatContext* ctx) const override;
  int AVFormatContextGetNbStreams(const AVFormatContext* ctx) const override;
  AVStream* AVFormatContextGetStreamByIdx(const AVFormatContext* ctx, int idx) const override;
  AVIOContext* AVFormatContextGetPb(const AVFormatContext* ctx) const override;
  int AVFormatContextGetFlags(const AVFormatContext* ctx) const override;
  int64_t AVFormatContextGetStartTime(const AVFormatContext* ctx) const override;
  int64_t AVFormatContextGetDuration(const AVFormatContext* ctx) const override;
  const AVInputFormat* AVFormatContextGetInputFormat(const AVFormatContext* ctx) const override;
  const AVOutputFormat* AVFormatContextGetOutputFormat(const AVFormatContext* ctx) const override;
  char* AVFormatContextGetUrl(const AVFormatContext* ctx) const override;

  void AVFormatContextSetFlags(AVFormatContext* ctx, int flags) const override;
  void AVFormatContextSetPb(AVFormatContext* ctx, AVIOContext* avio_ctx) const override;
  void AVFormatContextSetProbeSize(AVFormatContext* ctx, int64_t probesize) const override;
  void AVFormatContextSetMaxAnalyzeDuration(AVFormatContext* ctx, int64_t max_analyze_duration) const override;
  void AVFormatContextSetInterruptCallback(AVFormatContext* ctx, int (*callback)(void*), void *opaque) const override;
  void AVFormatContextSetUrl(AVFormatContext* ctx, char *url) const override;
  void AVFormatContextSetStartTime(AVFormatContext *ctx, int64_t start_time) const override;
  void AVFormatContextSetAvoidNegativeTs(AVFormatContext *ctx, int avoid_negative_ts) const override;


  int AVInputFormatGetFlags(const AVInputFormat* iformat) const override;
  int AVOutputFormatGetFlags(const AVOutputFormat* oformat) const override;

  int64_t AVPacketGetPts(const AVPacket* pkt) const override;
  int64_t AVPacketGetDts(const AVPacket* pkt) const override;
  void* AVPacketGetData(const AVPacket* pkt) const override;
  int AVPacketGetSize(const AVPacket* pkt) const override;
  int AVPacketGetStreamIndex(const AVPacket* pkt) const override;
  int AVPacketGetFlags(const AVPacket* pkt) const override;
  AVBufferRef* AVPacketGetBuf(const AVPacket* pkt) const override;
  int64_t AVPacketGetPos(const AVPacket* pkt) const override;
  int64_t AVPacketGetDuration(const AVPacket* pkt) const override;
  cmf::MediaTimeBase AVPacketGetTimeBase(AVPacket* pkt) const override;

  void AVPacketSetPts(AVPacket* pkt, int64_t pts) const override;
  void AVPacketSetDts(AVPacket* pkt, int64_t dts) const override;
  void AVPacketSetData(AVPacket* pkt, void* data) const override;
  void AVPacketSetSize(AVPacket* pkt, int size) const override;
  void AVPacketSetStreamIndex(AVPacket* pkt, int stream_idx) const override;
  void AVPacketSetFlags(AVPacket* pkt, int flags) const override;
  void AVPacketSetBuf(AVPacket* pkt, AVBufferRef* buf) const override;
  void AVPacketSetPos(AVPacket* pkt, int64_t pos) const override;
  void AVPacketSetDuration(AVPacket* pkt, int64_t duration) const override;
  void AVPacketSetTimeBase(AVPacket* pkt, cmf::MediaTimeBase tb) const override;

  int AVCodecContextGetChannels(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetSampleFormat(const AVCodecContext* codec_context) const override;

  int AVCodecContextGetSampleRate(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetWidth(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetHeight(const AVCodecContext* codec_context) const override;
  const AVCodec* AVCodecContextGetCodec(const AVCodecContext* codec_context) const override;
  cmf::MediaTimeBase AVCodecContextGetFramerate(const AVCodecContext* codec_context) const override;
  cmf::MediaTimeBase AVCodecContextGetTimeBase(const AVCodecContext* codec_context) const override;
  int64_t AVCodecContextGetBitRate(const AVCodecContext* codec_context) const override;
  int64_t AVCodecContextGetRcMaxRate(const AVCodecContext* codec_context) const override;
  cmf::MediaTimeBase AVCodecContextGetPktTimeBase(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetPixFmt(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetCodecId(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetFrameSize(const AVCodecContext* codec_context) const override;
  AVBufferRef* AVCodecContextGetHwFramesCtx(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetFlags(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetFlags2(const AVCodecContext* codec_context) const override;

  int AVCodecContextGetGopSize(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetKeyintMin(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetMaxBFrames(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetRcBufferSize(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetMeCmp(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetMeMethod(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetQMax(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetQMin(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetMaxQDiff(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetMeSubpelQuality(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetMeRange(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetThreadType(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetThreadCount(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetSlices(const AVCodecContext* codec_context) const override;
  float AVCodecContextGetIQuantFactor(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetRefs(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetTrellis(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetSampleFmt(const AVCodecContext* codec_context) const override;
  uint64_t AVCodecContextGetChannelLayout(const AVCodecContext* codec_context) const override;
  void* AVCodecContextGetOpaque(const AVCodecContext* codec_context) const override;
  AVBufferRef* AVCodecContextGetHwDeviceCtx(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetSwPixFmt(const AVCodecContext* codec_context) const override;
  float AVCodecContextGetQCompress(const AVCodecContext* codec_context) const override;
  void* AVCodecContextGetPrivData(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetDelay(const AVCodecContext* codec_context) const override;
  int AVCodecContextGetStrictStdCompliance(const AVCodecContext* codec_context) const override;

  void AVCodecContextSetGopSize(AVCodecContext* codec_context, int gop_size) const override;
  void AVCodecContextSetKeyintMin(AVCodecContext* codec_context, int keyint_min) const override;
  void AVCodecContextSetMaxBFrames(AVCodecContext* codec_context, int max_b_frames) const override;
  void AVCodecContextSetRcBufferSize(AVCodecContext* codec_context, int rc_buffer_size) const override;
  void AVCodecContextSetMeCmp(AVCodecContext* codec_context, int me_cmp) const override;
  void AVCodecContextSetMeMethod(AVCodecContext* codec_context, int me_method) const override;
  void AVCodecContextSetQMax(AVCodecContext* codec_context, int qmax) const override;
  void AVCodecContextSetQMin(AVCodecContext* codec_context, int qmin) const override;
  void AVCodecContextSetMaxQDiff(AVCodecContext* codec_context, int max_qdiff) const override;
  void AVCodecContextSetMeSubpelQuality(AVCodecContext* codec_context, int me_subpel_quality) const override;
  void AVCodecContextSetMeRange(AVCodecContext* codec_context, int me_range) const override;
  void AVCodecContextSetThreadType(AVCodecContext* codec_context, int thread_type) const override;
  void AVCodecContextSetThreadCount(AVCodecContext* codec_context, int thread_count) const override;
  void AVCodecContextSetSlices(AVCodecContext* codec_context, int slices) const override;
  void AVCodecContextSetIQuantFactor(AVCodecContext* codec_context, float iquant_factor) const override;
  void AVCodecContextSetRefs(AVCodecContext* codec_context, int refs) const override;
  void AVCodecContextSetTrellis(AVCodecContext* codec_context, int trellis) const override;
  void AVCodecContextSetSampleFmt(AVCodecContext* codec_context, int sample_fmt) const override;
  void AVCodecContextSetChannelLayout(AVCodecContext* codec_context, uint64_t channel_layout) const override;
  void AVCodecContextSetRcMaxAvailableVbvUse(AVCodecContext* codec_context, float max_available_vbv_use) const override;
  void AVCodecContextSetSampleRate(AVCodecContext* codec_context, int sample_rate) const override;
  void AVCodecContextSetChannels(AVCodecContext* codec_context, int channels) const override;
  void AVCodecContextSetTimeBase(AVCodecContext* codec_context, cmf::MediaTimeBase timebase) const override;
  void AVCodecContextSetBitRate(AVCodecContext* codec_context, int64_t bitrate) const override;
  void AVCodecContextSetRcMaxRate(AVCodecContext* codec_context, int64_t rc_max_rate) const override;
  void AVCodecContextSetPktTimeBase(AVCodecContext* codec_context, cmf::MediaTimeBase pkt_time_base) const override;
  void AVCodecContextSetFrameRate(AVCodecContext* codec_context, cmf::MediaTimeBase frame_rate) const override;
  void AVCodecContextSetDelay(AVCodecContext* codec_context, int delay) const override;
  void AVCodecContextSetStrictStdCompliance(AVCodecContext* codec_context, int value) const override;
  void AVCodecContextSetPixFmt(AVCodecContext* codec_context, int pixfmt) const override;
  void AVCodecContextSetWidth(AVCodecContext* codec_context, int width) const override;
  void AVCodecContextSetHeight(AVCodecContext* codec_context, int height) const override;
  void AVCodecContextSetProfile(AVCodecContext* codec_context, int profile) const override;
  void AVCodecContextSetFlags(AVCodecContext* codec_context, int flags) const override;
  void AVCodecContextSetFlags2(AVCodecContext* codec_context, int flags2) const override;
  void AVCodecContextSetOpaque(AVCodecContext* codec_context, void* opaque) const override;
  void AVCodecContextSetHwFramesCtx(AVCodecContext* codec_context, AVBufferRef* hw_frames_ctx_buf) const override;
  void AVCodecContextSetHwDeviceCtx(AVCodecContext* codec_context, AVBufferRef* hw_device_ctx_buf) const override;
  void AVCodecContextSetSwPixFmt(AVCodecContext* codec_context, int sw_pix_fmt) const override;
  void AVCodecContextSetQCompress(AVCodecContext* codec_context, float qcompress) const override;
  void AVCodecContextSetFrameSize(AVCodecContext* codec_context, int frame_size) const override;

  const char* AVCodecGetName(const AVCodec* codec) const override;
  const char* AVCodecGetLongName(const AVCodec* codec) const override;
  int AVCodecGetCapabilities(const AVCodec* codec) const override;
  int AVCodecGetId(const AVCodec* codec) const override;
  const int* AVCodecGetPixFmts(const AVCodec* codec) const override;
  const int* AVCodecGetSampleFmts(const AVCodec* codec) const override;
  void AVCodecGetSupportedFrameRates(const AVCodec* codec, std::vector<cmf::MediaTimeBase>& out_framerates) const override;
  const int* AVCodecGetSupportedSampleRates(const AVCodec* codec) const override;
  int AVCodecGetType(const AVCodec* codec) const override;

  size_t AVFrameSizeof() const override;

  int AVFrameGetSampleRate(const AVFrame* avframe) const override;
  int AVFrameGetWidth(const AVFrame* avframe) const override;
  int AVFrameGetHeight(const AVFrame* avframe) const override;
  int AVFrameGetFormat(const AVFrame* avframe) const override;
  int64_t AVFrameGetPts(const AVFrame* avframe) const override;
  int AVFrameGetNbSamples(const AVFrame* avframe) const override;
  int AVFrameGetKeyFrame(const AVFrame* avframe) const override;
  int AVFrameGetFlags(const AVFrame* avframe) const override;
  int AVFrameGetPictType(const AVFrame* avframe) const override;
  int AVFrameGetChannels(const AVFrame* avframe) const override;
  void* AVFrameGetOpaque(const AVFrame* avframe) const override;
  uint8_t* AVFrameGetData(const AVFrame* avframe, int idx) const override;
  int AVFrameGetLineSize(const AVFrame* avframe, int idx) const override;
  uint8_t * const *AVFrameGetDataPtr(const AVFrame* avframe) const override;
  const int *AVFrameGetLineSizePtr(const AVFrame* avframe) const override;
  uint64_t AVFrameGetChannelLayout(const AVFrame* avframe) const override;
  int AVFrameGetPktSize(const AVFrame* avframe) const override;
  int64_t AVFrameGetPktPos(const AVFrame* avframe) const override;
  int64_t AVFrameGetPktDuration(const AVFrame* avframe) const override;
  int64_t AVFrameGetPktDts(const AVFrame* avframe) const override;
  int64_t AVFrameGetPktPts(const AVFrame* avframe) const override;
  AVBufferRef* AVFrameGetBuf(const AVFrame* avframe, int idx) const override;
  AVBufferRef** AVFrameGetExtendedBuf(const AVFrame* avframe) const override;
  AVDictionary* AVFrameGetMetaData(const AVFrame* avframe) const override;
  AVFrameSideData** AVFrameGetSideData(const AVFrame* avframe) const override;
  AVBufferRef* AVFrameGetHwFramesCtx(const AVFrame* avframe) const override;
  uint8_t** AVFrameGetExtendedData(const AVFrame* avframe) const override;

  void AVFrameSetSampleRate(AVFrame* avframe, int sample_rate) const override;
  void AVFrameSetWidth(AVFrame* avframe, int width) const override;
  void AVFrameSetHeight(AVFrame* avframe, int height) const override;
  void AVFrameSetFormat(AVFrame* avframe, int format) const override;
  void AVFrameSetPts(AVFrame* avframe, int64_t pts) const override;
  void AVFrameSetNbSamples(AVFrame* avframe, int nb_samples) const override;
  void AVFrameSetKeyFrame(AVFrame* avframe, int keyframe) const override;
  void AVFrameSetFlags(AVFrame* avframe, int flags) const override;
  void AVFrameSetPictType(AVFrame* avframe, int pict_type) const override;
  void AVFrameSetChannels(AVFrame* avframe, int channels) const override;
  void AVFrameSetOpaque(AVFrame* avframe, void* opaque) const override;
  void AVFrameSetPktSize(AVFrame* avframe, int pkt_size) const override;
  void AVFrameSetPktPos(AVFrame* avframe, int64_t pkt_pos) const override;
  void AVFrameSetPktDuration(AVFrame* avframe, int64_t pkt_duration) const override;
  void AVFrameSetPktDts(AVFrame* avframe, int64_t pkt_dts) const override;
  void AVFrameSetPktPts(AVFrame* avframe, int64_t pkt_pts) const override;
  void AVFrameSetData(AVFrame* avframe, int idx, uint8_t* data) const override;
  void AVFrameSetLineSize(AVFrame* avframe, int idx, int linesize) const override;
  void AVFrameSetChannelLayout(AVFrame* avframe, uint64_t channel_layout) const override;
  void AVFrameSetBuf(AVFrame* avframe, int idx, AVBufferRef* buf) const override;
  void AVFrameSetExtendedBuf(AVFrame* avframe, AVBufferRef** extended_buf) const override;
  void AVFrameSetMetaData(AVFrame* avframe, AVDictionary* metadata) const override;
  void AVFrameSetSideData(AVFrame* avframe, AVFrameSideData** side_data) const override;
  void AVFrameSetHwFramesCtx(AVFrame* avframe, AVBufferRef* hw_frames_ctx) const override;
  void AVFrameSetExtendedData(AVFrame* avframe, uint8_t** extended_data) const override;


  int AVCodecParametersGetCodecType(const AVCodecParameters* codecpar) const override;
  int AVCodecParametersGetWidth(const AVCodecParameters* codecpar) const override;
  int AVCodecParametersGetHeight(const AVCodecParameters* codecpar) const override;
  int AVCodecParametersGetFormat(const AVCodecParameters* codecpar) const override;
  int AVCodecParametersGetChannels(const AVCodecParameters* codecpar) const override;
  int AVCodecParametersGetSampleRate(const AVCodecParameters* codecpar) const override;
  int AVCodecParametersGetExtraDataSize(const AVCodecParameters* codecpar) const override;
  uint8_t* AVCodecParametersGetExtraData(const AVCodecParameters* codecpar) const override;
  int64_t AVCodecParametersGetBitRate(const AVCodecParameters* codecpar) const override;
  int AVCodecParametersGetProfile(const AVCodecParameters* codecpar) const override;
  int AVCodecParametersGetCodecId(const AVCodecParameters* codecpar) const override;
  int AVCodecParametersGetVideoDelay(const AVCodecParameters* codecpar) const override;
  uint32_t AVCodecParametersGetCodecTag(const AVCodecParameters* codecpar) const override;
  int AVCodecParametersGetFrameSize(const AVCodecParameters* codecpar) const override;
  int AVCodecParametersGetBitsPerCodedSample(const AVCodecParameters* codecpar) const override;
  int AVCodecParametersGetBitsPerRawSample(const AVCodecParameters* codecpar) const override;

  void AVCodecParametersSetCodecType(AVCodecParameters* codecpar, int codec_type) const override;
  void AVCodecParametersSetWidth(AVCodecParameters* codecpar, int width) const override;
  void AVCodecParametersSetHeight(AVCodecParameters* codecpar, int height) const override;
  void AVCodecParametersSetFormat(AVCodecParameters* codecpar, int format) const override;
  void AVCodecParametersSetChannels(AVCodecParameters* codecpar, int channels) const override;
  void AVCodecParametersSetSampleRate(AVCodecParameters* codecpar, int sample_rate) const override;
  void AVCodecParametersSetExtraDataSize(AVCodecParameters* codecpar, int size) const override;
  void AVCodecParametersSetExtraData(AVCodecParameters* codecpar, uint8_t *extradata) const override;
  void AVCodecParametersSetBitRate(AVCodecParameters* codecpar, int64_t bitrate) const override;
  void AVCodecParametersSetProfile(AVCodecParameters* codecpar, int profile) const override;
  void AVCodecParametersSetCodecId(AVCodecParameters* codecpar, int codec_id) const override;
  void AVCodecParametersSetVideoDelay(AVCodecParameters* codecpar, int video_delay) const override;
  void AVCodecParametersSetCodecTag(AVCodecParameters* codecpar, uint32_t codec_tag) const override;
  void AVCodecParametersSetFrameSize(AVCodecParameters* codecpar, int frame_size) const override;
  void AVCodecParametersSetBitsPerCodedSample(AVCodecParameters* codecpar, int bits_per_coded_sample) const override;
  void AVCodecParametersSetBitsPerRawSample(AVCodecParameters* codecpar, int bits_per_raw_sample) const override;

  uint8_t* AVBufferRefGetData(const AVBufferRef* bufferref) const override;
  int AVBufferRefGetSize(const AVBufferRef* bufferref) const override;

  int AVHWConfigGetPixFmt(const AVCodecHWConfig* hwconfig) const override;
  int AVHWConfigGetDeviceType(const AVCodecHWConfig* hwconfig) const override;
  int AVHWConfigGetMethods(const AVCodecHWConfig* hwconfig) const override;

  size_t AVCPBPropertiesSizeof() const override;
  int AVCPBPropertiesGetBufferSize(const AVCPBProperties* props) const override;
  uint64_t AVCPBPropertiesGetVbvDelay(const AVCPBProperties* props) const override;
  int64_t AVCPBPropertiesGetMaxBitrate(const AVCPBProperties* props) const override;
  int64_t AVCPBPropertiesGetMinBitrate(const AVCPBProperties* props) const override;
  int64_t AVCPBPropertiesGetAvgBitrate(const AVCPBProperties* props) const override;

  void AVCPBPropertiesSetBufferSize(AVCPBProperties* props, int buffer_size) const override;
  void AVCPBPropertiesSetVbvDelay(AVCPBProperties* props, uint64_t vbv_delay) const override;
  void AVCPBPropertiesSetMaxBitrate(AVCPBProperties* props, int64_t max_bitrate) const override;
  void AVCPBPropertiesSetMinBitrate(AVCPBProperties* props, int64_t min_bitrate) const override;
  void AVCPBPropertiesSetAvgBitrate(AVCPBProperties* props, int64_t avg_bitrate) const override;

  int AVHWFramesContextGetFormat(const AVHWFramesContext* hwframes_ctx) const override;
  int AVHWFramesContextGetSwFormat(const AVHWFramesContext* hwframes_ctx) const override;
  int AVHWFramesContextGetWidth(const AVHWFramesContext* hwframes_ctx) const override;
  int AVHWFramesContextGetHeight(const AVHWFramesContext* hwframes_ctx) const override;

  void AVHWFramesContextSetFormat(AVHWFramesContext* hwframes_ctx, int format) const override;
  void AVHWFramesContextSetSwFormat(AVHWFramesContext* hwframes_ctx, int sw_format) const override;
  void AVHWFramesContextSetWidth(AVHWFramesContext* hwframes_ctx, int width) const override;
  void AVHWFramesContextSetHeight(AVHWFramesContext* hwframes_ctx, int height) const override;
  void AVHWFramesContextSetInitialPoolSize(AVHWFramesContext* hwframes_ctx, int initial_pool_size) const override;

  AVHWAccel* AVHWAccelNew() override;

  const char* AVHWAccelGetName(const AVHWAccel* hwaccel) const override;
  int AVHWAccelGetId(const AVHWAccel* hwaccel) const override;
  int AVHWAccelGetType(const AVHWAccel* hwaccel) const override;
  int AVHWAccelGetPixFmt(const AVHWAccel* hwaccel) const override;
  int AVHWAccelGetCapabilities(const AVHWAccel* hwaccel) const override;

  void AVHWAccelSetName(AVHWAccel* hwaccel, const char* name) const override;
  void AVHWAccelSetId(AVHWAccel* hwaccel, int id) const override;
  void AVHWAccelSetType(AVHWAccel* hwaccel, int type) const override;
  void AVHWAccelSetPixFmt(AVHWAccel* hwaccel, int pix_fmt) const override;
  void AVHWAccelSetCapabilities(AVHWAccel* hwaccel, int capabilities) const override;

  const char* AVPixFmtDescriptorGetName(const AVPixFmtDescriptor* descr) const override;
  const char* AVPixFmtDescriptorGetAlias(const AVPixFmtDescriptor* descr) const override;
  int AVPixFmtDescriptorGetNbComponents(const AVPixFmtDescriptor* descr) const override;
  uint64_t AVPixFmtDescriptorGetFlags(const AVPixFmtDescriptor* descr) const override;


  AVChannelLayout* AVChannelLayoutAllocate() const override;
  void AVChannelLayoutFree(AVChannelLayout* layout) const override;

  int AVChannelLayoutGetOrder(const AVChannelLayout* layout) const override;
  int AVChannelLayoutGetNbChannels(const AVChannelLayout* layout) const override;
  void* AVChannelLayoutGetOpaque(const AVChannelLayout* layout) const override;
  uint64_t AVChannelLayoutGetMask(const AVChannelLayout* layout) const override;
  const AVChannelCustom* AVChannelLayoutGetMap(const AVChannelLayout* layout) const override;

  void AVChannelLayoutSetOrder(AVChannelLayout* layout, int order) const override;
  void AVChannelLayoutSetNbChannels(AVChannelLayout* layout, int nb_channels) const override;
  void AVChannelLayoutSetOpaque(AVChannelLayout* layout, void* opaque) const override;
  void AVChannelLayoutSetMask(AVChannelLayout* layout, uint64_t mask) const override;

private:
  std::weak_ptr<IAvcModuleProvider> module_provider_;
};

class AVC_MODULE_DATA_WRAPPER_FACTORYNAME : public virtual IAvcModuleDataWrapperFactory {
public:
  AVC_MODULE_DATA_WRAPPER_FACTORYNAME();
  virtual ~AVC_MODULE_DATA_WRAPPER_FACTORYNAME() = default;

  const AvcModuleVersion& GetVersion() const override;
  std::shared_ptr<IAvcModuleDataWrapper> Create(std::shared_ptr<IAvcModuleProvider> module_provider) override;
private:
  AvcModuleVersion version_;
};


}//namespace detail
}//namespace cmf

#endif // AVC_MODULE_DATA_WRAPPER_H
