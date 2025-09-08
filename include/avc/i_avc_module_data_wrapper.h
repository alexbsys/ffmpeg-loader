
#ifndef I_AVC_MODULE_DATA_WRAPPER_HEADER
#define I_AVC_MODULE_DATA_WRAPPER_HEADER

#include <media/media_timebase.h>

namespace avc {

// avcodec structures definition
struct AVCodecParameters;
struct AVHWAccel;
struct AVCodecContext;
// enum AVLockOp;
struct AVPacket;
struct AVCodec;
// enum AVCodecID;
struct AVCodecHWConfig;

// avformat structures definition
struct AVFormatContext;
struct AVInputFormat;
struct AVOutputFormat;
struct AVStream;
struct AVIOContext;
struct AVIOInterruptCB;
struct AVCPBProperties;

// avutil structures definition
struct AVHWFramesConstraints;
struct AVHWFramesContext;
struct AVPixFmtDescriptor;

struct AVDictionary;
struct AVFrame;
struct AVFrameSideData;

struct AVDictionaryEntry;
struct AVBufferRef;

// swscale
struct SwsFilter;
struct SwsContext;

// swresample
struct SwrContext;
	
struct IAvcModuleDataWrapper {
  virtual ~IAvcModuleDataWrapper() = default;

  // AVStream
  virtual AVCodecParameters* AVStreamGetCodecPar(const AVStream* stream) const = 0;
  virtual cmf::MediaTimeBase AVStreamGetTimeBase(const AVStream* stream) const = 0;
  virtual cmf::MediaTimeBase AVStreamGetFrameRate(const AVStream* stream) const = 0;
  virtual cmf::MediaTimeBase AVStreamGetAvgFrameRage(const AVStream* stream) const = 0;
  virtual void AVStreamSetTimeBase(AVStream* stream, cmf::MediaTimeBase tb) const = 0;
  virtual void AVStreamSetFrameRate(AVStream* stream, cmf::MediaTimeBase framerate) const = 0;
  virtual void AVStreamSetAvgFrameRate(AVStream* stream, cmf::MediaTimeBase framerate) const = 0;
  virtual void AVStreamSetStartTime(AVStream* stream, int64_t start_time) const = 0;
  virtual int AVStreamGetIndex(const AVStream* stream) const = 0;
  virtual void AVStreamSetIndex(AVStream* stream, int index) const = 0;
  virtual int AVStreamGetId(const AVStream* stream) const = 0;
  virtual void AVStreamSetId(AVStream* stream, int id) const = 0;

  // AVFormatContext
  virtual int AVFormatContextGetAvoidNegativeTs(const AVFormatContext* ctx) const = 0;
  virtual int AVFormatContextGetNbStreams(const AVFormatContext* ctx) const = 0;
  virtual AVStream* AVFormatContextGetStreamByIdx(const AVFormatContext* ctx, int idx) const = 0;
  virtual AVIOContext* AVFormatContextGetPb(const AVFormatContext* ctx) const = 0;
  virtual int AVFormatContextGetFlags(const AVFormatContext* ctx) const = 0;
  virtual int64_t AVFormatContextGetStartTime(const AVFormatContext* ctx) const = 0;
  virtual int64_t AVFormatContextGetDuration(const AVFormatContext* ctx) const = 0;
  virtual const AVInputFormat* AVFormatContextGetInputFormat(const AVFormatContext* ctx) const = 0;
  virtual const AVOutputFormat* AVFormatContextGetOutputFormat(const AVFormatContext* ctx) const = 0;
  virtual char* AVFormatContextGetUrl(const AVFormatContext* ctx) const = 0;

  virtual void AVFormatContextSetPb(AVFormatContext* ctx, AVIOContext* avio_ctx) const = 0;
  virtual void AVFormatContextSetFlags(AVFormatContext* ctx, int flags) const = 0;
  virtual void AVFormatContextSetProbeSize(AVFormatContext* ctx, int64_t probesize) const = 0;
  virtual void AVFormatContextSetMaxAnalyzeDuration(AVFormatContext* ctx, int64_t max_analyze_duration) const = 0;
  virtual void AVFormatContextSetInterruptCallback(AVFormatContext* ctx, int (*callback)(void*), void *opaque) const = 0;
  virtual void AVFormatContextSetUrl(AVFormatContext* ctx, char* url) const = 0;
  virtual void AVFormatContextSetStartTime(AVFormatContext *ctx, int64_t start_time) const  = 0;
  virtual void AVFormatContextSetAvoidNegativeTs(AVFormatContext* ctx, int avoid_negative_ts) const = 0;

  // AVIOContext
  virtual unsigned char* AVIOContextGetBuffer(const AVIOContext* ctx) const = 0;
  virtual void AVIOContextSetBuffer(AVIOContext* ctx, unsigned char* buffer) const = 0;

  // AVInputFormat
  virtual int AVInputFormatGetFlags(const AVInputFormat* iformat) const  = 0;

  // AVOutputFormat
  virtual int AVOutputFormatGetFlags(const AVOutputFormat* oformat) const = 0;

  // AVPacket
  virtual int64_t AVPacketGetPts(const AVPacket* pkt) const = 0;
  virtual int64_t AVPacketGetDts(const AVPacket* pkt) const = 0;
  virtual void* AVPacketGetData(const AVPacket* pkt) const = 0;
  virtual int AVPacketGetSize(const AVPacket* pkt) const = 0;
  virtual int AVPacketGetStreamIndex(const AVPacket* pkt) const = 0;
  virtual int AVPacketGetFlags(const AVPacket* pkt) const = 0;
  virtual AVBufferRef* AVPacketGetBuf(const AVPacket* pkt) const = 0;
  virtual int64_t AVPacketGetPos(const AVPacket* pkt) const = 0;
  virtual int64_t AVPacketGetDuration(const AVPacket* pkt) const = 0;

  virtual void AVPacketSetPts(AVPacket* pkt, int64_t pts) const = 0;
  virtual void AVPacketSetDts(AVPacket* pkt, int64_t dts) const = 0;
  virtual void AVPacketSetData(AVPacket* pkt, void* data) const = 0;
  virtual void AVPacketSetSize(AVPacket* pkt, int size) const = 0;
  virtual void AVPacketSetStreamIndex(AVPacket* pkt, int stream_idx) const = 0;
  virtual void AVPacketSetFlags(AVPacket* pkt, int flags) const = 0;
  virtual void AVPacketSetBuf(AVPacket* pkt, AVBufferRef* buf) const = 0;
  virtual void AVPacketSetPos(AVPacket* pkt, int64_t pos) const = 0;
  virtual void AVPacketSetDuration(AVPacket* pkt, int64_t duration) const = 0;

  // AVCodecContext
  virtual int AVCodecContextGetChannels(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetSampleFormat(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetSampleRate(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetWidth(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetHeight(const AVCodecContext* codec_context) const = 0;
  virtual const AVCodec* AVCodecContextGetCodec(const AVCodecContext* codec_context) const = 0;
  virtual cmf::MediaTimeBase AVCodecContextGetFramerate(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetDelay(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetStrictStdCompliance(const AVCodecContext* codec_context) const = 0;
  virtual cmf::MediaTimeBase AVCodecContextGetTimeBase(const AVCodecContext* codec_context) const = 0;
  virtual int64_t AVCodecContextGetBitRate(const AVCodecContext* codec_context) const = 0;
  virtual int64_t AVCodecContextGetRcMaxRate(const AVCodecContext* codec_context) const = 0;
  virtual cmf::MediaTimeBase AVCodecContextGetPktTimeBase(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetPixFmt(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetCodecId(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetFrameSize(const AVCodecContext* codec_context) const = 0;
  virtual AVBufferRef* AVCodecContextGetHwFramesCtx(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetFlags(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetFlags2(const AVCodecContext* codec_context) const = 0;

  virtual int AVCodecContextGetGopSize(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetKeyintMin(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetMaxBFrames(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetRcBufferSize(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetMeCmp(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetMeMethod(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetQMax(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetQMin(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetMaxQDiff(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetMeSubpelQuality(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetMeRange(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetThreadType(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetThreadCount(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetSlices(const AVCodecContext* codec_context) const = 0;
  virtual float AVCodecContextGetIQuantFactor(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetRefs(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetTrellis(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetSampleFmt(const AVCodecContext* codec_context) const = 0;
  virtual uint64_t AVCodecContextGetChannelLayout(const AVCodecContext* codec_context) const = 0;
  virtual void* AVCodecContextGetOpaque(const AVCodecContext* codec_context) const = 0;
  virtual AVBufferRef* AVCodecContextGetHwDeviceCtx(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetSwPixFmt(const AVCodecContext* codec_context) const = 0;
  virtual float AVCodecContextGetQCompress(const AVCodecContext* codec_context) const = 0;
  virtual void* AVCodecContextGetPrivData(const AVCodecContext* codec_context) const = 0;

  virtual void AVCodecContextSetGopSize(AVCodecContext* codec_context, int gop_size) const = 0;
  virtual void AVCodecContextSetKeyintMin(AVCodecContext* codec_context, int keyint_min) const = 0;
  virtual void AVCodecContextSetMaxBFrames(AVCodecContext* codec_context, int max_b_frames) const = 0;
  virtual void AVCodecContextSetRcBufferSize(AVCodecContext* codec_context, int rc_buffer_size) const = 0;
  virtual void AVCodecContextSetMeCmp(AVCodecContext* codec_context, int me_cmp) const = 0;
  virtual void AVCodecContextSetMeMethod(AVCodecContext* codec_context, int me_method) const = 0;
  virtual void AVCodecContextSetQMax(AVCodecContext* codec_context, int qmax) const = 0;
  virtual void AVCodecContextSetQMin(AVCodecContext* codec_context, int qmin) const = 0;
  virtual void AVCodecContextSetMaxQDiff(AVCodecContext* codec_context, int max_qdiff) const = 0;
  virtual void AVCodecContextSetMeSubpelQuality(AVCodecContext* codec_context, int me_subpel_quality) const = 0;
  virtual void AVCodecContextSetMeRange(AVCodecContext* codec_context, int me_range) const = 0;
  virtual void AVCodecContextSetThreadType(AVCodecContext* codec_context, int thread_type) const = 0;
  virtual void AVCodecContextSetThreadCount(AVCodecContext* codec_context, int thread_count) const = 0;
  virtual void AVCodecContextSetSlices(AVCodecContext* codec_context, int slices) const = 0;
  virtual void AVCodecContextSetIQuantFactor(AVCodecContext* codec_context, float iquant_factor) const = 0;
  virtual void AVCodecContextSetRefs(AVCodecContext* codec_context, int refs) const = 0;
  virtual void AVCodecContextSetTrellis(AVCodecContext* codec_context, int trellis) const = 0;
  virtual void AVCodecContextSetSampleFmt(AVCodecContext* codec_context, int sample_fmt) const = 0;
  virtual void AVCodecContextSetChannelLayout(AVCodecContext* codec_context, uint64_t channel_layout) const = 0;
  virtual void AVCodecContextSetRcMaxAvailableVbvUse(AVCodecContext* codec_contex, float max_available_vbv_use) const = 0;
  virtual void AVCodecContextSetSampleRate(AVCodecContext* codec_context, int sample_rate) const = 0;
  virtual void AVCodecContextSetChannels(AVCodecContext* codec_context, int channels) const = 0;
  virtual void AVCodecContextSetOpaque(AVCodecContext* codec_context, void* opaque) const = 0;
  virtual void AVCodecContextSetHwFramesCtx(AVCodecContext* codec_context, AVBufferRef* hw_frames_ctx_buf) const = 0;
  virtual void AVCodecContextSetHwDeviceCtx(AVCodecContext* codec_context, AVBufferRef* hw_device_ctx_buf) const = 0;
  virtual void AVCodecContextSetRcMaxRate(AVCodecContext* codec_context, int64_t rc_max_rate) const = 0;
  virtual void AVCodecContextSetTimeBase(AVCodecContext* codec_context, cmf::MediaTimeBase timebase) const = 0;
  virtual void AVCodecContextSetBitRate(AVCodecContext* codec_context, int64_t bitrate) const = 0;
  virtual void AVCodecContextSetPktTimeBase(AVCodecContext* codec_context, cmf::MediaTimeBase pkt_time_base) const = 0;
  virtual void AVCodecContextSetFrameRate(AVCodecContext* codec_context, cmf::MediaTimeBase frame_rate) const = 0;
  virtual void AVCodecContextSetDelay(AVCodecContext* codec_context, int delay) const = 0;
  virtual void AVCodecContextSetStrictStdCompliance(AVCodecContext* codec_context, int value) const = 0;
  virtual void AVCodecContextSetPixFmt(AVCodecContext* codec_context, int pixfmt) const = 0;
  virtual void AVCodecContextSetWidth(AVCodecContext* codec_context, int width) const = 0;
  virtual void AVCodecContextSetHeight(AVCodecContext* codec_context, int height) const = 0;
  virtual void AVCodecContextSetProfile(AVCodecContext* codec_context, int profile) const = 0;
  virtual void AVCodecContextSetFlags(AVCodecContext* codec_context, int flags) const = 0;
  virtual void AVCodecContextSetFlags2(AVCodecContext* codec_context, int flags2) const = 0;
  virtual void AVCodecContextSetSwPixFmt(AVCodecContext* codec_context, int sw_pix_fmt) const = 0;
  virtual void AVCodecContextSetQCompress(AVCodecContext* codec_context, float qcompress) const = 0;
  virtual void AVCodecContextSetFrameSize(AVCodecContext* codec_context, int frame_size) const = 0;


  // AVCodec
  virtual const char* AVCodecGetName(const AVCodec* codec) const = 0;
  virtual const char* AVCodecGetLongName(const AVCodec* codec) const = 0;
  virtual int AVCodecGetCapabilities(const AVCodec* codec) const = 0;
  virtual int AVCodecGetId(const AVCodec* codec) const = 0;
  virtual const int* AVCodecGetPixFmts(const AVCodec* codec) const = 0;
  virtual const int* AVCodecGetSampleFmts(const AVCodec* codec) const = 0;
  virtual void AVCodecGetSupportedFrameRates(const AVCodec* codec, std::vector<cmf::MediaTimeBase>& out_framerates) const = 0;
  virtual const int* AVCodecGetSupportedSampleRates(const AVCodec* codec) const = 0;
  virtual int AVCodecGetType(const AVCodec* codec) const = 0;

  // AVFrame
  virtual size_t AVFrameSizeof() const  = 0;
  virtual int AVFrameGetSampleRate(const AVFrame* avframe) const = 0;
  virtual int AVFrameGetWidth(const AVFrame* avframe) const = 0;
  virtual int AVFrameGetHeight(const AVFrame* avframe) const = 0;
  virtual int AVFrameGetFormat(const AVFrame* avframe) const = 0;
  virtual int64_t AVFrameGetPts(const AVFrame* avframe) const = 0;
  virtual int AVFrameGetNbSamples(const AVFrame* avframe) const = 0;
  virtual int AVFrameGetKeyFrame(const AVFrame* avframe) const = 0;
  virtual int AVFrameGetFlags(const AVFrame* avframe) const = 0;
  virtual int AVFrameGetPictType(const AVFrame* avframe) const = 0;
  virtual int AVFrameGetChannels(const AVFrame* avframe) const = 0;
  virtual void* AVFrameGetOpaque(const AVFrame* avframe) const = 0;
  virtual uint8_t* AVFrameGetData(const AVFrame* avframe, int idx) const = 0;
  virtual uint8_t* const* AVFrameGetDataPtr(const AVFrame* avframe) const = 0;
  virtual void AVFrameSetData(AVFrame* avframe, int idx, uint8_t* data) const = 0;
  virtual const int* AVFrameGetLineSizePtr(const AVFrame* avframe) const = 0;
  virtual int AVFrameGetLineSize(const AVFrame* avframe, int idx) const = 0;
  virtual uint64_t AVFrameGetChannelLayout(const AVFrame* avframe) const =  0;
  virtual int AVFrameGetPktSize(const AVFrame* avframe) const = 0;
  virtual int64_t AVFrameGetPktPos(const AVFrame* avframe) const = 0;
  virtual int64_t AVFrameGetPktDuration(const AVFrame* avframe) const = 0;
  virtual int64_t AVFrameGetPktDts(const AVFrame* avframe) const = 0;
  virtual int64_t AVFrameGetPktPts(const AVFrame* avframe) const = 0;
  virtual AVBufferRef* AVFrameGetBuf(const AVFrame* avframe, int idx) const = 0;
  virtual AVBufferRef** AVFrameGetExtendedBuf(const AVFrame* avframe) const = 0;
  virtual AVDictionary* AVFrameGetMetaData(const AVFrame* avframe) const = 0;
  virtual AVFrameSideData** AVFrameGetSideData(const AVFrame* avframe) const = 0;
  virtual AVBufferRef* AVFrameGetHwFramesCtx(const AVFrame* avframe) const = 0;
  virtual uint8_t** AVFrameGetExtendedData(const AVFrame* avframe) const = 0;

  virtual void AVFrameSetSampleRate(AVFrame* avframe, int sample_rate) const = 0;
  virtual void AVFrameSetWidth(AVFrame* avframe, int width) const = 0;
  virtual void AVFrameSetHeight(AVFrame* avframe, int height) const = 0;
  virtual void AVFrameSetFormat(AVFrame* avframe, int format) const = 0;
  virtual void AVFrameSetPts(AVFrame* avframe, int64_t pts) const = 0;
  virtual void AVFrameSetNbSamples(AVFrame* avframe, int nb_samples) const = 0;
  virtual void AVFrameSetKeyFrame(AVFrame* avframe, int keyframe) const = 0;
  virtual void AVFrameSetFlags(AVFrame* avframe, int flags) const = 0;
  virtual void AVFrameSetPictType(AVFrame* avframe, int pict_type) const = 0;
  virtual void AVFrameSetChannels(AVFrame* avframe, int channels) const = 0;
  virtual void AVFrameSetOpaque(AVFrame* avframe, void* opaque) const = 0;
  virtual void AVFrameSetLineSize(AVFrame* avframe, int idx, int linesize) const = 0;
  virtual void AVFrameSetChannelLayout(AVFrame* avframe, uint64_t channel_layout) const = 0;
  virtual void AVFrameSetPktSize(AVFrame* avframe, int pkt_size) const = 0;
  virtual void AVFrameSetPktPos(AVFrame* avframe, int64_t pkt_pos) const = 0;
  virtual void AVFrameSetPktDuration(AVFrame* avframe, int64_t pkt_duration) const = 0;
  virtual void AVFrameSetPktDts(AVFrame* avframe, int64_t pkt_dts) const = 0;
  virtual void AVFrameSetPktPts(AVFrame* avframe, int64_t pkt_pts) const = 0;
  virtual void AVFrameSetBuf(AVFrame* avframe, int idx, AVBufferRef* buf) const = 0;
  virtual void AVFrameSetExtendedBuf(AVFrame* avframe, AVBufferRef** extended_buf) const = 0;
  virtual void AVFrameSetMetaData(AVFrame* avframe, AVDictionary* metadata) const = 0;
  virtual void AVFrameSetSideData(AVFrame* avframe, AVFrameSideData** side_data) const = 0;
  virtual void AVFrameSetHwFramesCtx(AVFrame* avframe, AVBufferRef* hw_frames_ctx) const = 0;
  virtual void AVFrameSetExtendedData(AVFrame* avframe, uint8_t** extended_data) const = 0;

  // AVCodecParameters
  virtual int AVCodecParametersGetCodecType(const AVCodecParameters* codecpar) const = 0;
  virtual int AVCodecParametersGetWidth(const AVCodecParameters* codecpar) const = 0;
  virtual int AVCodecParametersGetHeight(const AVCodecParameters* codecpar) const = 0;
  virtual int AVCodecParametersGetFormat(const AVCodecParameters* codecpar) const = 0;
  virtual int AVCodecParametersGetChannels(const AVCodecParameters* codecpar) const = 0;
  virtual int AVCodecParametersGetSampleRate(const AVCodecParameters* codecpar) const = 0;
  virtual int AVCodecParametersGetExtraDataSize(const AVCodecParameters* codecpar) const = 0;
  virtual uint8_t* AVCodecParametersGetExtraData(const AVCodecParameters* codecpar) const = 0;
  virtual int64_t AVCodecParametersGetBitRate(const AVCodecParameters* codecpar) const = 0;
  virtual int AVCodecParametersGetProfile(const AVCodecParameters* codecpar) const = 0;
  virtual int AVCodecParametersGetCodecId(const AVCodecParameters* codecpar) const = 0;
  virtual int AVCodecParametersGetVideoDelay(const AVCodecParameters* codecpar) const = 0;
  virtual uint32_t AVCodecParametersGetCodecTag(const AVCodecParameters* codecpar) const = 0;

  virtual void AVCodecParametersSetCodecType(AVCodecParameters* codecpar, int codec_type) const = 0;
  virtual void AVCodecParametersSetWidth(AVCodecParameters* codecpar, int width) const = 0;
  virtual void AVCodecParametersSetHeight(AVCodecParameters* codecpar, int height) const = 0;
  virtual void AVCodecParametersSetFormat(AVCodecParameters* codecpar, int format) const = 0;
  virtual void AVCodecParametersSetChannels(AVCodecParameters* codecpar, int channels) const = 0;
  virtual void AVCodecParametersSetSampleRate(AVCodecParameters* codecpar, int sample_rate) const = 0;
  virtual void AVCodecParametersSetExtraDataSize(AVCodecParameters* codecpar, int size) const = 0;
  virtual void AVCodecParametersSetExtraData(AVCodecParameters* codecpar, uint8_t* extradata) const = 0;
  virtual void AVCodecParametersSetBitRate(AVCodecParameters* codecpar, int64_t bitrate) const = 0;
  virtual void AVCodecParametersSetProfile(AVCodecParameters* codecpar, int profile) const = 0;
  virtual void AVCodecParametersSetCodecId(AVCodecParameters* codecpar, int codec_id) const = 0;
  virtual void AVCodecParametersSetVideoDelay(AVCodecParameters* codecpar, int video_delay) const = 0;
  virtual void AVCodecParametersSetCodecTag(AVCodecParameters* codecpar, uint32_t codec_tag) const = 0;

  // AVBufferRef
  virtual uint8_t* AVBufferRefGetData(const AVBufferRef* bufferref) const = 0;
  virtual int AVBufferRefGetSize(const AVBufferRef* bufferref) const = 0;

  // AVHWConfig
  virtual int AVHWConfigGetPixFmt(const AVCodecHWConfig* hwconfig) const = 0;
  virtual int AVHWConfigGetDeviceType(const AVCodecHWConfig* hwconfig) const = 0;
  virtual int AVHWConfigGetMethods(const AVCodecHWConfig* hwconfig) const = 0;

  // AVCPBProperties
  virtual size_t AVCPBPropertiesSizeof() const = 0;
  virtual int AVCPBPropertiesGetBufferSize(const AVCPBProperties* props) const = 0;
  virtual uint64_t AVCPBPropertiesGetVbvDelay(const AVCPBProperties* props) const = 0;
  virtual int64_t AVCPBPropertiesGetMaxBitrate(const AVCPBProperties* props) const = 0;
  virtual int64_t AVCPBPropertiesGetMinBitrate(const AVCPBProperties* props) const = 0;
  virtual int64_t AVCPBPropertiesGetAvgBitrate(const AVCPBProperties* props) const = 0;

  virtual void AVCPBPropertiesSetBufferSize(AVCPBProperties* props, int buffer_size) const = 0;
  virtual void AVCPBPropertiesSetVbvDelay(AVCPBProperties* props, uint64_t vbv_delay) const = 0;
  virtual void AVCPBPropertiesSetMaxBitrate(AVCPBProperties* props, int64_t max_bitrate) const = 0;
  virtual void AVCPBPropertiesSetMinBitrate(AVCPBProperties* props, int64_t min_bitrate) const = 0;
  virtual void AVCPBPropertiesSetAvgBitrate(AVCPBProperties* props, int64_t avg_bitrate) const = 0;

  // AVHWFramesContext
  virtual int AVHWFramesContextGetFormat(const AVHWFramesContext* hwframes_ctx) const = 0;
  virtual int AVHWFramesContextGetSwFormat(const AVHWFramesContext* hwframes_ctx) const = 0;
  virtual int AVHWFramesContextGetWidth(const AVHWFramesContext* hwframes_ctx) const = 0;
  virtual int AVHWFramesContextGetHeight(const AVHWFramesContext* hwframes_ctx) const = 0;

  virtual void AVHWFramesContextSetFormat(AVHWFramesContext* hwframes_ctx, int format) const = 0;
  virtual void AVHWFramesContextSetSwFormat(AVHWFramesContext* hwframes_ctx, int sw_format) const = 0;
  virtual void AVHWFramesContextSetWidth(AVHWFramesContext* hwframes_ctx, int width) const = 0;
  virtual void AVHWFramesContextSetHeight(AVHWFramesContext* hwframes_ctx, int height) const = 0;
  virtual void AVHWFramesContextSetInitialPoolSize(AVHWFramesContext* hwframes_ctx, int initial_pool_size) const = 0;

  // AVHWAccel
  virtual AVHWAccel* AVHWAccelNew() = 0;

  virtual const char* AVHWAccelGetName(const AVHWAccel* hwaccel) const = 0;
  virtual int AVHWAccelGetId(const AVHWAccel* hwaccel) const = 0;
  virtual int AVHWAccelGetType(const AVHWAccel* hwaccel) const = 0;
  virtual int AVHWAccelGetPixFmt(const AVHWAccel* hwaccel) const = 0;
  virtual int AVHWAccelGetCapabilities(const AVHWAccel* hwaccel) const = 0;

  virtual void AVHWAccelSetName(AVHWAccel* hwaccel, const char* name) const = 0;
  virtual void AVHWAccelSetId(AVHWAccel* hwaccel, int id) const = 0;
  virtual void AVHWAccelSetType(AVHWAccel* hwaccel, int type) const = 0;
  virtual void AVHWAccelSetPixFmt(AVHWAccel* hwaccel, int pix_fmt) const = 0;
  virtual void AVHWAccelSetCapabilities(AVHWAccel* hwaccel, int capabilities) const = 0;

  //AVPixFmtDescriptor
  virtual const char* AVPixFmtDescriptorGetName(const AVPixFmtDescriptor* descr) const = 0;
  virtual const char* AVPixFmtDescriptorGetAlias(const AVPixFmtDescriptor* descr) const = 0;
  virtual int AVPixFmtDescriptorGetNbComponents(const AVPixFmtDescriptor* descr) const = 0;
  virtual uint64_t AVPixFmtDescriptorGetFlags(const AVPixFmtDescriptor* descr) const = 0;
};

}//namespace avc

#endif //I_AVC_MODULE_DATA_WRAPPER_HEADER
