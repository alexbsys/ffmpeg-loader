#ifndef I_AVC_MODULE_PROVIDER_H
#define I_AVC_MODULE_PROVIDER_H

#include <cstdarg>
#include <cstdint>
#include <memory>
#include <vector>

#ifndef MEDIA_TIMEBASE_DEFINED
struct MediaTimeBase {
  int num_;
  int den_;

  MediaTimeBase() : num_(0), den_(1) {}
  MediaTimeBase(int num, int den) : num_(num), den_(den) {}
};

#define MEDIA_TIMEBASE_DEFINED 1
#endif //MEDIA_TIMEBASE_DEFINED

namespace cmf {

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
// enum AVSampleFormat;
// enum AVPixelFormat;

struct AVBufferRef;

// swscale
struct SwsFilter;
struct SwsContext;

// swresample
struct SwrContext;

struct IAvcModuleDataWrapper;

enum AvcLogLevel {
  kAvcLogLevel_Error = 0,
  kAvcLogLevel_Warning = 1,
  kAvcLogLevel_Info = 2,
  kAvcLogLevel_Debug = 3
};

enum AvcCompliance {
  kAvcCompliance_VeryStrict = 2,
  kAvcCompliance_Strict = 1,
  kAvcCompliance_Normal = 0,
  kAvcCompliance_Unofficial = -1,
  kAvcCompliance_Experimental = -2
};

enum AVLockOp {
  AV_LOCK_CREATE,  ///< Create a mutex
  AV_LOCK_OBTAIN,  ///< Lock the mutex
  AV_LOCK_RELEASE, ///< Unlock the mutex
  AV_LOCK_DESTROY, ///< Free mutex resources
};

struct AvcModuleVersion {
  unsigned int avcodec_version_;
  unsigned int avutil_version_;
  unsigned int avformat_version_;
  unsigned int avdevice_version_;
  unsigned int swscale_version_;
  unsigned int swresample_version_;

  AvcModuleVersion()
    : avcodec_version_(0)
    , avutil_version_(0)
    , avformat_version_(0)
    , avdevice_version_(0)
    , swscale_version_(0)
    , swresample_version_(0) {
  }

  bool operator ==(const AvcModuleVersion& other) const {
    return
        avcodec_version_ == other.avcodec_version_
        && avutil_version_ == other.avutil_version_
        && avformat_version_ == other.avformat_version_
        && avdevice_version_ == other.avdevice_version_
        && swscale_version_ == other.swscale_version_
        && swresample_version_ == other.swresample_version_;
  }
};


struct IAvcVideoPixelFormatConverter {
  virtual ~IAvcVideoPixelFormatConverter() = default;

  virtual int VideoPixelFormatToAVPixelFormat(enum VideoPixelFormat video_pixel_format) const = 0;
  virtual enum VideoPixelFormat AVPixelFormatToVideoPixelFormat(int av_pixel_format) const = 0;
};


struct IAvcModuleProvider {
  virtual ~IAvcModuleProvider() = default;

  virtual bool IsAvCodecLoaded() const = 0;
  virtual bool IsAvFormatLoaded() const = 0;
  virtual bool IsAvUtilLoaded() const = 0;
  virtual bool IsAvDeviceLoaded() const = 0;
  virtual bool IsSwScaleLoaded() const = 0;
  virtual bool IsSwResampleLoaded() const = 0;


  // avcodec
  virtual unsigned avcodec_version() = 0;

  virtual int avcodec_encode_video2(AVCodecContext *avctx, AVPacket *avpkt,
                                    const AVFrame *frame, int *got_packet_ptr) = 0;

  virtual size_t av_get_codec_tag_string(char *buf, size_t buf_size,
                                         unsigned int codec_tag) = 0;
  virtual int avcodec_parameters_copy(AVCodecParameters *dst,
                                      const AVCodecParameters *src) = 0;
  virtual int avcodec_parameters_from_context(AVCodecParameters *par,
                                              const AVCodecContext *codec) = 0;
  virtual void avcodec_parameters_free(AVCodecParameters **par) = 0;
  virtual AVCodecParameters *avcodec_parameters_alloc(void) = 0;
  virtual int avcodec_parameters_to_context(AVCodecContext *codec,
                                            const AVCodecParameters *par) = 0;
  virtual void av_register_hwaccel(AVHWAccel *hwaccel) = 0;
  virtual AVHWAccel *av_hwaccel_next(const AVHWAccel *hwaccel) = 0;
  virtual int av_lockmgr_register(int (*cb)(void **mutex, int /*enum AVLockOp*/ op)) = 0;
  virtual AVPacket *av_packet_alloc(void) = 0;
  virtual AVPacket *av_packet_clone(AVPacket *src) = 0;
  virtual void av_packet_free(AVPacket **pkt) = 0;
  virtual void av_init_packet(AVPacket *pkt) = 0;
  virtual int av_new_packet(AVPacket *pkt, int size) = 0;
  virtual void av_packet_ref(AVPacket *dst, const AVPacket* src) = 0;
  virtual void av_packet_unref(AVPacket *pkt) = 0;
  virtual AVCodecContext *avcodec_alloc_context3(const AVCodec *codec) = 0;
  virtual void avcodec_free_context(AVCodecContext **avctx) = 0;
  virtual AVCodec *avcodec_find_decoder(int /*enum AVCodecID*/ id) = 0;
  virtual AVCodec *avcodec_find_decoder_by_name(const char *name) = 0;
  virtual AVCodec *avcodec_find_encoder(int /*enum AVCodecID*/ id) = 0;
  virtual AVCodec *avcodec_find_encoder_by_name(const char *name) = 0;
  virtual void avcodec_flush_buffers(AVCodecContext *avctx) = 0;

  virtual const AVCodec *av_codec_iterate(void **opaque) = 0;
  virtual const AVCodecHWConfig *avcodec_get_hw_config(const AVCodec *codec,
                                                       int index) = 0;

  virtual const char *avcodec_get_name(int /*enum AVCodecID*/ id) = 0;
  virtual int av_codec_is_encoder(const AVCodec *codec) = 0;
  virtual int av_codec_is_decoder(const AVCodec *codec) = 0;
  virtual int avcodec_open2(AVCodecContext *avctx, const AVCodec *codec,
                            AVDictionary **options) = 0;
  virtual int avcodec_receive_frame(AVCodecContext *avctx, AVFrame *frame) = 0;
  virtual int avcodec_send_frame(AVCodecContext *avctx, const AVFrame *frame) = 0;
  virtual int avcodec_receive_packet(AVCodecContext *avctx, AVPacket *avpkt) = 0;
  virtual int avcodec_send_packet(AVCodecContext *avctx, const AVPacket *avpkt) = 0;
  virtual void avcodec_register_all() = 0;

  // avformat functions
  virtual unsigned avformat_version() = 0;

  virtual void av_dump_format(AVFormatContext *ic, int index, const char *url,
                              int is_output) = 0;
  virtual AVInputFormat *av_find_input_format(const char *short_name) = 0;

  virtual AVOutputFormat *av_guess_format(const char *short_name, const char *filename,
                                          const char *mime_type) = 0;

  virtual int av_read_frame(AVFormatContext *s, AVPacket *pkt) = 0;
  virtual int av_read_play(AVFormatContext *s) = 0;
  virtual int av_read_pause(AVFormatContext *s) = 0;

  virtual void av_register_all(void) = 0;
  virtual int avformat_flush(AVFormatContext *s) = 0;
  virtual int av_seek_frame(AVFormatContext *s, int stream_index, int64_t timestamp,
                            int flags) = 0;
  virtual int av_write_frame(AVFormatContext *s, AVPacket *pkt) = 0;
  virtual int av_interleaved_write_frame(AVFormatContext *s, AVPacket *pkt) = 0;

  virtual int av_write_trailer(AVFormatContext *s) = 0;

  virtual AVFormatContext *avformat_alloc_context(void) = 0;
  virtual int avformat_alloc_output_context2(AVFormatContext **ctx,
                                             AVOutputFormat *oformat,
                                             const char *format_name,
                                             const char *filename) = 0;

  virtual void avformat_free_context(AVFormatContext *s) = 0;

  virtual void avformat_close_input(AVFormatContext **s) = 0;
  virtual int avformat_find_stream_info(AVFormatContext *ic, AVDictionary **options) = 0;

  virtual int avformat_network_init(void) = 0;
  virtual int avformat_network_deinit(void) = 0;

  virtual AVStream *avformat_new_stream(AVFormatContext *s, const AVCodec *c) = 0;

  virtual int av_stream_add_side_data(AVStream *st, int type,
                              uint8_t *data, size_t size) = 0;
  virtual uint8_t *av_stream_new_side_data(AVStream *stream,
                                   int type, int size) = 0;
  virtual uint8_t *av_stream_get_side_data(const AVStream *stream,
                                   int type, int *size) = 0;


  virtual int avformat_open_input(AVFormatContext **ps, const char *url,
                                  AVInputFormat *fmt, AVDictionary **options) = 0;

  virtual int avformat_write_header(AVFormatContext *s, AVDictionary **options) = 0;

  virtual AVIOContext *avio_alloc_context(
    unsigned char *buffer, int buffer_size, int write_flag, void *opaque,
    int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),
    int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),
    int64_t (*seek)(void *opaque, int64_t offset, int whence)) = 0;

  virtual void avio_context_free(AVIOContext** s) = 0;

  virtual int avio_close(AVIOContext *s) = 0;
  virtual int avio_closep(AVIOContext **s) = 0;
  virtual void avio_flush(AVIOContext *s) = 0;

  virtual int avio_open2(AVIOContext **s, const char *url, int flags,
                         const AVIOInterruptCB *int_cb, AVDictionary **options) = 0;

  // avutil
  virtual unsigned avutil_version() = 0;

  virtual int av_samples_get_buffer_size(int *linesize, int nb_channels, int nb_samples,
                                         int /*enum AVSampleFormat*/ sample_fmt,
                                         int align) = 0;

  virtual int64_t av_rescale_rnd(int64_t a, int64_t b, int64_t c,
                                 int /*enum AVRounding*/ rnd) = 0;

  virtual int av_samples_alloc(uint8_t **audio_data, int *linesize, int nb_channels,
                               int nb_samples, int /*enum AVSampleFormat*/ sample_fmt,
                               int /*(0 = default, 1 = no alignment)*/ align) = 0;

  virtual int av_samples_alloc_array_and_samples(
    uint8_t ***audio_data, int *linesize, int nb_channels, int nb_samples,
    int /*enum AVSampleFormat*/ sample_fmt,
    int /*(0 = default, 1 = no alignment)*/ align) = 0;

  virtual int av_opt_set_int(void *obj, const char *name, int64_t val,
                             int search_flags) = 0;
  virtual int av_opt_set_sample_fmt(void *obj, const char *name,
                                    int /*enum AVSampleFormat*/ fmt,
                                    int search_flags) = 0;

  virtual AVBufferRef *av_hwdevice_ctx_alloc(int /*enum AVHWDeviceType*/ type) = 0;
  virtual int av_hwdevice_ctx_init(AVBufferRef *ref) = 0;
  virtual AVBufferRef *av_hwframe_ctx_alloc(AVBufferRef *device_ctx) = 0;

  virtual int /*enum AVHWDeviceType*/ av_hwdevice_find_type_by_name(const char *name) = 0;

  virtual const char *av_hwdevice_get_type_name(int /*enum AVHWDeviceType*/ type) = 0;

  virtual int /*enum AVHWDeviceType*/ av_hwdevice_iterate_types(
    int /*enum AVHWDeviceType*/ prev) = 0;

  virtual int av_hwdevice_ctx_create(AVBufferRef **device_ctx,
                                     int /*enum AVHWDeviceType*/ type, const char *device,
                                     AVDictionary *opts, int flags) = 0;

  virtual int av_hwframe_ctx_init(AVBufferRef *ref) = 0;
  virtual int av_hwframe_get_buffer(AVBufferRef *hwframe_ctx, AVFrame *frame,
                                    int flags) = 0;
  virtual int av_hwframe_transfer_data(AVFrame *dst, const AVFrame *src, int flags) = 0;

  virtual int av_hwframe_transfer_get_formats(AVBufferRef *hwframe_ctx,
                                              int /*enum AVHWFrameTransferDirection*/ dir,
                                              int /*enum AVPixelFormat*/ **formats,
                                              int flags) = 0;

  virtual void *av_hwdevice_hwconfig_alloc(AVBufferRef *device_ctx) = 0;

  virtual AVHWFramesConstraints *av_hwdevice_get_hwframe_constraints(
    AVBufferRef *ref, const void *hwconfig) = 0;

  virtual void av_hwframe_constraints_free(AVHWFramesConstraints **constraints) = 0;
  virtual int av_hwframe_map(AVFrame *dst, const AVFrame *src, int flags) = 0;

  virtual int av_dict_set(AVDictionary **pm, const char *key, const char *value,
                          int flags) = 0;
  virtual int av_dict_set_int(AVDictionary **pm, const char *key, int64_t value,
                              int flags) = 0;
  virtual AVDictionaryEntry *av_dict_get(const AVDictionary *m, const char *key,
                                         const AVDictionaryEntry *prev, int flags) = 0;
  virtual int av_dict_count(const AVDictionary *m) = 0;
  virtual void av_dict_free(AVDictionary **m) = 0;

  virtual AVFrame *av_frame_alloc(void) = 0;
  virtual void av_frame_free(AVFrame **frame) = 0;

  virtual int av_frame_get_buffer(AVFrame *frame, int align) = 0;

  virtual int av_frame_get_channels(const AVFrame *frame) = 0;
  virtual void av_frame_set_channels(AVFrame *frame, int val) = 0;
  virtual int64_t av_frame_get_pkt_duration(const AVFrame *frame) = 0;
  virtual void av_frame_set_pkt_duration(AVFrame *frame, int64_t val) = 0;
  virtual int64_t av_frame_get_pkt_pos(const AVFrame *frame) = 0;
  virtual void av_frame_set_pkt_pos(AVFrame *frame, int64_t val) = 0;
  virtual int av_frame_get_sample_rate(const AVFrame *frame) = 0;
  virtual void av_frame_set_sample_rate(AVFrame *frame, int val) = 0;

  virtual void av_free(void *ptr) = 0;
  virtual void av_freep(void *ptr) = 0;

  virtual int av_get_bytes_per_sample(int /*enum AVSampleFormat*/ sample_fmt) = 0;

  virtual int av_image_copy_to_buffer(uint8_t *dst, int dst_size,
                                      const uint8_t *const src_data[4],
                                      const int src_linesize[4],
                                      int /*enum AVPixelFormat*/ pix_fmt, int width,
                                      int height, int align) = 0;

  virtual int av_image_fill_arrays(uint8_t *dst_data[4], int dst_linesize[4],
                                   const uint8_t *src, int /*enum AVPixelFormat*/ pix_fmt,
                                   int width, int height, int align) = 0;

  virtual int av_image_get_buffer_size(int /*enum AVPixelFormat*/ pix_fmt, int width,
                                       int height, int align) = 0;

  virtual void av_log_default_callback(void *avcl, int level, const char *fmt,
                                       va_list vl) = 0;

  virtual void av_log_set_callback(void (*callback)(void *, int, const char *,
                                                    va_list)) = 0;

  virtual void av_log_set_level(int level) = 0;

  virtual void *av_malloc(size_t size) = 0;

  virtual char *av_strdup(const char *str) = 0;

  virtual AVBufferRef *av_buffer_create(uint8_t *data, int size,
                                        void (*free)(void *opaque, uint8_t *data),
                                        void *opaque, int flags) = 0;

  virtual int av_buffer_is_writable(const AVBufferRef *buf) = 0;
  virtual void *av_buffer_get_opaque(const AVBufferRef *buf) = 0;
  virtual int av_buffer_get_ref_count(const AVBufferRef *buf) = 0;
  virtual int av_buffer_make_writable(AVBufferRef **buf) = 0;

  virtual int av_buffer_realloc(AVBufferRef **pbuf, int size) = 0;
  virtual void av_buffer_unref(AVBufferRef **buf) = 0;
  virtual AVBufferRef *av_buffer_ref(AVBufferRef *buf) = 0;

  virtual int av_pix_fmt_count_planes(int /*enum AVPixelFormat*/ pix_fmt) = 0;
  virtual const AVPixFmtDescriptor *av_pix_fmt_desc_get(int /*enum AVPixelFormat*/ pix_fmt) = 0;


  virtual int av_sample_fmt_is_planar(int /*enum AVSampleFormat*/ sample_fmt) = 0;

  virtual int av_samples_set_silence(uint8_t **audio_data, int offset, int nb_samples,
                                     int nb_channels,
                                     int /*enum AVSampleFormat*/ sample_fmt) = 0;

  virtual int av_strerror(int errnum, char *errbuf, size_t errbuf_size) = 0;

  virtual uint64_t av_get_channel_layout(const char *name) = 0;
  virtual int av_get_channel_layout_nb_channels(uint64_t channel_layout) = 0;
  virtual int64_t av_get_default_channel_layout(int nb_channels) = 0;

  virtual int av_get_channel_layout_channel_index(uint64_t channel_layout,
                                                  uint64_t channel) = 0;

  virtual uint64_t av_channel_layout_extract_channel(uint64_t channel_layout,
                                                     int index) = 0;

  virtual const char *av_get_channel_name(uint64_t channel) = 0;

  virtual const char *av_get_channel_description(uint64_t channel) = 0;
  virtual int av_get_standard_channel_layout(unsigned index, uint64_t *layout,
                                             const char **name) = 0;

  // swscale
  virtual unsigned swscale_version() = 0;

  virtual void sws_freeContext(struct SwsContext *swsContext) = 0;

  virtual struct SwsContext *sws_getContext(
    int srcW, int srcH, int /*enum AVPixelFormat*/ srcFormat, int dstW, int dstH,
    int /*enum AVPixelFormat*/ dstFormat, int flags, SwsFilter *srcFilter,
    SwsFilter *dstFilter, const double *param) = 0;

  virtual int sws_scale(struct SwsContext *c, const uint8_t *const srcSlice[],
                        const int srcStride[], int srcSliceY, int srcSliceH,
                        uint8_t *const dst[], const int dstStride[]) = 0;

  // swresample
  virtual unsigned swresample_version() = 0;

  virtual struct SwrContext *swr_alloc() = 0;
  virtual int swr_init(struct SwrContext *s) = 0;
  virtual int swr_is_initialized(struct SwrContext *s) = 0;
  virtual void swr_free(struct SwrContext **s) = 0;
  virtual void swr_close(struct SwrContext *s) = 0;
  virtual int swr_convert(struct SwrContext *s, uint8_t **out, int out_count,
                          const uint8_t **in, int in_count) = 0;
  virtual int64_t swr_get_delay(struct SwrContext *s, int64_t base) = 0;

  // avdevice
  virtual unsigned avdevice_version() = 0;
  virtual void avdevice_register_all() = 0;


  // wrap data structures
  virtual std::shared_ptr<IAvcModuleDataWrapper> d() const = 0;

  virtual std::shared_ptr<IAvcVideoPixelFormatConverter> GetVideoPixelFormatConverter() = 0;
};


struct IAvcModuleDataWrapper {
  virtual ~IAvcModuleDataWrapper() = default;

  // AVStream
  virtual AVCodecParameters* AVStreamGetCodecPar(const AVStream* stream) const = 0;
  virtual MediaTimeBase AVStreamGetTimeBase(const AVStream* stream) const = 0;
  virtual MediaTimeBase AVStreamGetFrameRate(const AVStream* stream) const = 0;
  virtual MediaTimeBase AVStreamGetAvgFrameRage(const AVStream* stream) const = 0;
  virtual void AVStreamSetTimeBase(AVStream* stream, MediaTimeBase tb) const = 0;
  virtual void AVStreamSetFrameRate(AVStream* stream, MediaTimeBase framerate) const = 0;
  virtual void AVStreamSetAvgFrameRate(AVStream* stream, MediaTimeBase framerate) const = 0;
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
  virtual MediaTimeBase AVCodecContextGetFramerate(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetDelay(const AVCodecContext* codec_context) const = 0;
  virtual int AVCodecContextGetStrictStdCompliance(const AVCodecContext* codec_context) const = 0;
  virtual MediaTimeBase AVCodecContextGetTimeBase(const AVCodecContext* codec_context) const = 0;
  virtual int64_t AVCodecContextGetBitRate(const AVCodecContext* codec_context) const = 0;
  virtual int64_t AVCodecContextGetRcMaxRate(const AVCodecContext* codec_context) const = 0;
  virtual MediaTimeBase AVCodecContextGetPktTimeBase(const AVCodecContext* codec_context) const = 0;
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
  virtual void AVCodecContextSetTimeBase(AVCodecContext* codec_context, MediaTimeBase timebase) const = 0;
  virtual void AVCodecContextSetBitRate(AVCodecContext* codec_context, int64_t bitrate) const = 0;
  virtual void AVCodecContextSetPktTimeBase(AVCodecContext* codec_context, MediaTimeBase pkt_time_base) const = 0;
  virtual void AVCodecContextSetFrameRate(AVCodecContext* codec_context, MediaTimeBase frame_rate) const = 0;
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
  virtual void AVCodecGetSupportedFrameRates(const AVCodec* codec, std::vector<MediaTimeBase>& out_framerates) const = 0;
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

struct IAvcModuleDataWrapperFactory {
  virtual ~IAvcModuleDataWrapperFactory() = default;

  virtual const AvcModuleVersion& GetVersion() const = 0;
  virtual std::shared_ptr<IAvcModuleDataWrapper> Create(std::shared_ptr<IAvcModuleProvider> module_provider) = 0;
};

}  // namespace cmf

#endif  // I_AVC_MODULE_PROVIDER_H
