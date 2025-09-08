#ifndef AVC_MODULE_PROVIDER_H
#define AVC_MODULE_PROVIDER_H

#include <memory>
#include <string>
#include <tools/i_dynamic_modules_loader.h>
#include <avc/i_avc_module_provider.h>
#include <avc/i_avc_module_load_handler.h>

namespace avc {
namespace detail {

class AvcModuleProvider : public virtual IAvcModuleProvider,
                          public std::enable_shared_from_this<AvcModuleProvider> {
 public:
  AvcModuleProvider(
    std::shared_ptr<cmf::IDynamicModulesLoader> modules_loader,
    std::shared_ptr<IAvcModuleLoadHandler> load_handler,
    const std::string &modules_path);

  AvcModuleProvider(
    std::shared_ptr<cmf::IDynamicModulesLoader> modules_loader,
    std::shared_ptr<IAvcModuleLoadHandler> load_handler,
    const std::string &modules_path,
    const std::string &avcodec_module_name,
    const std::string &avformat_module_name,
    const std::string &avutil_module_name,
    const std::string &avdevice_module_name,
    const std::string &swscale_module_name,
    const std::string &swresample_module_name,
    bool strict_modules_names = false);

  virtual ~AvcModuleProvider() override;

  void Load();
  void Unload();

  bool IsAvCodecLoaded() const override;
  bool IsAvFormatLoaded() const override;
  bool IsAvUtilLoaded() const override;
  bool IsAvDeviceLoaded() const override;
  bool IsSwScaleLoaded() const override;
  bool IsSwResampleLoaded() const override;

  std::shared_ptr<IAvcModuleLoadHandler> GetLoadHandler() const;
  void SetLoadHandler(std::shared_ptr<IAvcModuleLoadHandler> load_handler);

#ifdef AVC_LIBRARIES_STATIC_LINK
  void LoadStatically();
#endif /*AVC_LIBRARIES_STATIC_LINK*/


private:
  bool LoadAvModule(const char* name, void** handle, const std::string& module_name, const std::string& noversion_module_name);
  bool strict_modules_names_;

public:
  void SetupDataWrapper();

  /// \brief   Calculate FFmpeg libraries compatibility version score based on version difference
  /// \return  0 - full compatibility, some positive value - lesser values means more compatibility,
  ///          <0 - checked version cannot be used
  static int CalculateVersionsScore(const AvcModuleVersion& required, const AvcModuleVersion& checked);

  // avcodec
  unsigned avcodec_version() override;

  int avcodec_encode_video2(AVCodecContext *avctx, AVPacket *avpkt, const AVFrame *frame,
                            int *got_packet_ptr) override;

  size_t av_get_codec_tag_string(char *buf, size_t buf_size,
                                 unsigned int codec_tag) override;
  int avcodec_parameters_copy(AVCodecParameters *dst,
                              const AVCodecParameters *src) override;
  int avcodec_parameters_from_context(AVCodecParameters *par,
                                      const AVCodecContext *codec) override;
  void avcodec_parameters_free(AVCodecParameters **par) override;
  AVCodecParameters *avcodec_parameters_alloc(void) override;
  int avcodec_parameters_to_context(AVCodecContext *codec,
                                    const AVCodecParameters *par) override;
  void av_register_hwaccel(AVHWAccel *hwaccel) override;
  AVHWAccel *av_hwaccel_next(const AVHWAccel *hwaccel) override;
  int av_lockmgr_register(int (*cb)(void **mutex, int /*enum AVLockOp*/ op)) override;
  AVPacket *av_packet_alloc(void) override;
  AVPacket *av_packet_clone(AVPacket *src) override;
  void av_packet_free(AVPacket **pkt) override;
  void av_init_packet(AVPacket *pkt) override;
  int av_new_packet(AVPacket *pkt, int size) override;
  void av_packet_ref(AVPacket *dst, const AVPacket* src) override;
  void av_packet_unref(AVPacket *pkt) override;

  AVCodecContext *avcodec_alloc_context3(const AVCodec *codec) override;
  void avcodec_free_context(AVCodecContext **avctx) override;
  AVCodec *avcodec_find_decoder(int /*enum AVCodecID*/ id) override;
  AVCodec *avcodec_find_decoder_by_name(const char *name) override;
  AVCodec *avcodec_find_encoder(int /*enum AVCodecID*/ id) override;
  AVCodec *avcodec_find_encoder_by_name(const char *name) override;
  void avcodec_flush_buffers(AVCodecContext *avctx) override;

  const AVCodec *av_codec_iterate(void **opaque) override;
  const AVCodecHWConfig *avcodec_get_hw_config(const AVCodec *codec, int index) override;

  const char *avcodec_get_name(int /*enum AVCodecID*/ id) override;
  int av_codec_is_encoder(const AVCodec *codec) override;
  int av_codec_is_decoder(const AVCodec *codec) override;
  int avcodec_open2(AVCodecContext *avctx, const AVCodec *codec,
                    AVDictionary **options) override;
  int avcodec_receive_frame(AVCodecContext *avctx, AVFrame *frame) override;
  int avcodec_send_frame(AVCodecContext *avctx, const AVFrame *frame) override;
  int avcodec_receive_packet(AVCodecContext *avctx, AVPacket *avpkt) override;
  int avcodec_send_packet(AVCodecContext *avctx, const AVPacket *avpkt) override;
  void avcodec_register_all() override;

  // avformat functions
  unsigned avformat_version() override;

  void av_dump_format(AVFormatContext *ic, int index, const char *url,
                      int is_output) override;
  AVInputFormat *av_find_input_format(const char *short_name) override;

  AVOutputFormat *av_guess_format(const char *short_name, const char *filename,
                                  const char *mime_type) override;

  int av_read_frame(AVFormatContext *s, AVPacket *pkt) override;
  int av_read_play(AVFormatContext *s) override;
  int av_read_pause(AVFormatContext *s) override;

  void av_register_all(void) override;
  int avformat_flush(AVFormatContext *s) override;
  int av_seek_frame(AVFormatContext *s, int stream_index, int64_t timestamp,
                    int flags) override;
  int av_write_frame(AVFormatContext *s, AVPacket *pkt) override;
  int av_interleaved_write_frame(AVFormatContext *s, AVPacket *pkt) override;

  int av_write_trailer(AVFormatContext *s) override;

  AVFormatContext *avformat_alloc_context(void) override;
  int avformat_alloc_output_context2(AVFormatContext **ctx, AVOutputFormat *oformat,
                                     const char *format_name,
                                     const char *filename) override;

  void avformat_free_context(AVFormatContext *s) override;

  void avformat_close_input(AVFormatContext **s) override;
  int avformat_find_stream_info(AVFormatContext *ic, AVDictionary **options) override;

  int avformat_network_init(void) override;
  int avformat_network_deinit(void) override;

  AVStream *avformat_new_stream(AVFormatContext *s, const AVCodec *c) override;

  int av_stream_add_side_data(AVStream *st, int type,
                              uint8_t *data, size_t size) override;
  uint8_t *av_stream_new_side_data(AVStream *stream,
                                   int type, int size) override;
  uint8_t *av_stream_get_side_data(const AVStream *stream,
                                   int type, int *size) override;


  int avformat_open_input(AVFormatContext **ps, const char *url, AVInputFormat *fmt,
                          AVDictionary **options) override;

  int avformat_write_header(AVFormatContext *s, AVDictionary **options) override;

  AVIOContext *avio_alloc_context(
    unsigned char *buffer, int buffer_size, int write_flag, void *opaque,
    int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),
    int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),
    int64_t (*seek)(void *opaque, int64_t offset, int whence)) override;

  void avio_context_free(AVIOContext** s) override;

  int avio_close(AVIOContext *s) override;
  int avio_closep(AVIOContext **s) override;
  void avio_flush(AVIOContext *s) override;

  int avio_open2(AVIOContext **s, const char *url, int flags,
                 const AVIOInterruptCB *int_cb, AVDictionary **options) override;

  // avutils
  unsigned avutil_version() override;

  char *av_strdup(const char *s) override;

  int av_samples_get_buffer_size(int *linesize, int nb_channels, int nb_samples,
                                 int /*enum AVSampleFormat*/ sample_fmt,
                                 int align) override;

  int64_t av_rescale_rnd(int64_t a, int64_t b, int64_t c,
                         int /*enum AVRounding*/ rnd) override;

  int av_samples_alloc(uint8_t **audio_data, int *linesize, int nb_channels,
                       int nb_samples, int /*enum AVSampleFormat*/ sample_fmt,
                       int /*(0 = default, 1 = no alignment)*/ align) override;

  int av_samples_alloc_array_and_samples(
    uint8_t ***audio_data, int *linesize, int nb_channels, int nb_samples,
    int /*enum AVSampleFormat*/ sample_fmt,
    int /*(0 = default, 1 = no alignment)*/ align) override;

  int av_opt_set_int(void *obj, const char *name, int64_t val, int search_flags) override;
  int av_opt_set_sample_fmt(void *obj, const char *name, int /*enum AVSampleFormat*/ fmt,
                            int search_flags) override;

  AVBufferRef *av_hwdevice_ctx_alloc(int /*enum AVHWDeviceType*/ type) override;
  int av_hwdevice_ctx_init(AVBufferRef *ref) override;
  AVBufferRef *av_hwframe_ctx_alloc(AVBufferRef *device_ctx) override;

  int /*enum AVHWDeviceType*/ av_hwdevice_find_type_by_name(const char *name) override;

  const char *av_hwdevice_get_type_name(int /*enum AVHWDeviceType*/ type) override;

  int /*enum AVHWDeviceType*/ av_hwdevice_iterate_types(
    int /*enum AVHWDeviceType*/ prev) override;

  int av_hwdevice_ctx_create(AVBufferRef **device_ctx, int /*enum AVHWDeviceType*/ type,
                             const char *device, AVDictionary *opts, int flags) override;

  int av_hwframe_ctx_init(AVBufferRef *ref) override;
  int av_hwframe_get_buffer(AVBufferRef *hwframe_ctx, AVFrame *frame, int flags) override;
  int av_hwframe_transfer_data(AVFrame *dst, const AVFrame *src, int flags) override;

  int av_hwframe_transfer_get_formats(AVBufferRef *hwframe_ctx,
                                      int /*enum AVHWFrameTransferDirection*/ dir,
                                      int /*enum AVPixelFormat*/ **formats,
                                      int flags) override;

  void *av_hwdevice_hwconfig_alloc(AVBufferRef *device_ctx) override;

  AVHWFramesConstraints *av_hwdevice_get_hwframe_constraints(
    AVBufferRef *ref, const void *hwconfig) override;

  void av_hwframe_constraints_free(AVHWFramesConstraints **constraints) override;
  int av_hwframe_map(AVFrame *dst, const AVFrame *src, int flags) override;

  int av_dict_set(AVDictionary **pm, const char *key, const char *value,
                  int flags) override;
  int av_dict_set_int(AVDictionary **pm, const char *key, int64_t value,
                      int flags) override;
  AVDictionaryEntry *av_dict_get(const AVDictionary *m, const char *key,
                                 const AVDictionaryEntry *prev, int flags) override;
  int av_dict_count(const AVDictionary *m) override;
  void av_dict_free(AVDictionary **m) override;

  AVFrame *av_frame_alloc(void) override;
  void av_frame_free(AVFrame **frame) override;

  int av_frame_get_buffer(AVFrame *frame, int align) override;

  int av_frame_get_channels(const AVFrame *frame) override;
  void av_frame_set_channels(AVFrame *frame, int val) override;
  int64_t av_frame_get_pkt_duration(const AVFrame *frame) override;
  void av_frame_set_pkt_duration(AVFrame *frame, int64_t val) override;
  int64_t av_frame_get_pkt_pos(const AVFrame *frame) override;
  void av_frame_set_pkt_pos(AVFrame *frame, int64_t val) override;
  int av_frame_get_sample_rate(const AVFrame *frame) override;
  void av_frame_set_sample_rate(AVFrame *frame, int val) override;

  void av_free(void *ptr) override;
  void av_freep(void *ptr) override;

  int av_get_bytes_per_sample(int /*enum AVSampleFormat*/ sample_fmt) override;

  int av_image_copy_to_buffer(uint8_t *dst, int dst_size,
                              const uint8_t *const src_data[4], const int src_linesize[4],
                              int /*enum AVPixelFormat*/ pix_fmt, int width, int height,
                              int align) override;

  int av_image_fill_arrays(uint8_t *dst_data[4], int dst_linesize[4], const uint8_t *src,
                           int /*enum AVPixelFormat*/ pix_fmt, int width, int height,
                           int align) override;

  int av_image_get_buffer_size(int /*enum AVPixelFormat*/ pix_fmt, int width, int height,
                               int align) override;

  void av_log_default_callback(void *avcl, int level, const char *fmt,
                               va_list vl) override;

  void av_log_set_callback(void (*callback)(void *, int, const char *, va_list)) override;

  void av_log_set_level(int level) override;

  void *av_malloc(size_t size) override;

  AVBufferRef *av_buffer_create(uint8_t *data, int size,
                                void (*free)(void *opaque, uint8_t *data), void *opaque,
                                int flags) override;

  int av_buffer_is_writable(const AVBufferRef *buf) override;
  void *av_buffer_get_opaque(const AVBufferRef *buf) override;
  int av_buffer_get_ref_count(const AVBufferRef *buf) override;
  int av_buffer_make_writable(AVBufferRef **buf) override;

  int av_buffer_realloc(AVBufferRef **pbuf, int size) override;
  void av_buffer_unref(AVBufferRef **buf) override;
  AVBufferRef *av_buffer_ref(AVBufferRef *buf) override;

  int av_pix_fmt_count_planes(int /*enum AVPixelFormat*/ pix_fmt) override;
  const AVPixFmtDescriptor *av_pix_fmt_desc_get(int /*enum AVPixelFormat*/ pix_fmt) override;

  int av_sample_fmt_is_planar(int /*enum AVSampleFormat*/ sample_fmt) override;

  int av_samples_set_silence(uint8_t **audio_data, int offset, int nb_samples,
                             int nb_channels,
                             int /*enum AVSampleFormat*/ sample_fmt) override;

  int av_strerror(int errnum, char *errbuf, size_t errbuf_size) override;

  uint64_t av_get_channel_layout(const char *name) override;
  int av_get_channel_layout_nb_channels(uint64_t channel_layout) override;
  int64_t av_get_default_channel_layout(int nb_channels) override;

  int av_get_channel_layout_channel_index(uint64_t channel_layout,
                                          uint64_t channel) override;

  uint64_t av_channel_layout_extract_channel(uint64_t channel_layout, int index) override;

  const char *av_get_channel_name(uint64_t channel) override;

  const char *av_get_channel_description(uint64_t channel) override;
  int av_get_standard_channel_layout(unsigned index, uint64_t *layout,
                                     const char **name) override;

  // swscale
  unsigned swscale_version() override;

  void sws_freeContext(struct SwsContext *swsContext) override;

  struct SwsContext *sws_getContext(int srcW, int srcH,
                                    int /*enum AVPixelFormat*/ srcFormat, int dstW,
                                    int dstH, int /*enum AVPixelFormat*/ dstFormat,
                                    int flags, SwsFilter *srcFilter, SwsFilter *dstFilter,
                                    const double *param) override;

  int sws_scale(struct SwsContext *c, const uint8_t *const srcSlice[],
                const int srcStride[], int srcSliceY, int srcSliceH, uint8_t *const dst[],
                const int dstStride[]) override;

  // swresample
  unsigned swresample_version() override;

  struct SwrContext *swr_alloc() override;
  int swr_init(struct SwrContext *s) override;
  int swr_is_initialized(struct SwrContext *s) override;
  void swr_free(struct SwrContext **s) override;
  void swr_close(struct SwrContext *s) override;
  int swr_convert(struct SwrContext *s, uint8_t **out, int out_count, const uint8_t **in,
                  int in_count) override;
  int64_t swr_get_delay(struct SwrContext *s, int64_t base) override;

  // avdevice
  unsigned avdevice_version() override;
  void avdevice_register_all() override;

  // Wrappers for data structures
  std::shared_ptr<IAvcModuleDataWrapper> d() const override;

  // pixel format converter
  std::shared_ptr<IAvcVideoPixelFormatConverter> GetVideoPixelFormatConverter() override;

 private:
  void LoadAvCodecFunctions();
  void LoadAvFormatFunctions();
  void LoadAvUtilFunctions();
  void LoadAvDeviceFunctions();
  void LoadSwScaleFunctions();
  void LoadSwResampleFuctions();

  // avcodec
  unsigned (*avcodec_version_)(void);

  int (*avcodec_encode_video2_)(AVCodecContext *avctx, AVPacket *avpkt,
                                const AVFrame *frame, int *got_packet_ptr);

  size_t (*av_get_codec_tag_string_)(char *buf, size_t buf_size, unsigned int codec_tag);
  int (*avcodec_parameters_copy_)(AVCodecParameters *dst, const AVCodecParameters *src);
  int (*avcodec_parameters_from_context_)(AVCodecParameters *par,
                                          const AVCodecContext *codec);
  void (*avcodec_parameters_free_)(AVCodecParameters **par);
  AVCodecParameters *(*avcodec_parameters_alloc_)(void);
  int (*avcodec_parameters_to_context_)(AVCodecContext *codec,
                                        const AVCodecParameters *par);
  void (*av_register_hwaccel_)(AVHWAccel *hwaccel);
  AVHWAccel *(*av_hwaccel_next_)(const AVHWAccel *hwaccel);
  int (*av_lockmgr_register_)(int (*cb)(void **mutex, /*enum AVLockOp*/ int op));
  AVPacket *(*av_packet_alloc_)(void);
  AVPacket *(*av_packet_clone_)(AVPacket *src);
  void (*av_packet_free_)(AVPacket **pkt);
  void (*av_init_packet_)(AVPacket *pkt);
  int (*av_new_packet_)(AVPacket *pkt, int size);
  int (*av_packet_ref_)(AVPacket *dst, const AVPacket* src);
  void (*av_packet_unref_)(AVPacket *pkt);

  AVCodecContext *(*avcodec_alloc_context3_)(const AVCodec *codec);
  void (*avcodec_free_context_)(AVCodecContext **avctx);
  AVCodec *(*avcodec_find_decoder_)(/*enum AVCodecID*/ int id);
  AVCodec *(*avcodec_find_decoder_by_name_)(const char *name);
  AVCodec *(*avcodec_find_encoder_)(/*enum AVCodecID*/ int id);
  AVCodec *(*avcodec_find_encoder_by_name_)(const char *name);
  void (*avcodec_flush_buffers_)(AVCodecContext *avctx);

  const AVCodec *(*av_codec_iterate_)(void **opaque);
  const AVCodecHWConfig *(*avcodec_get_hw_config_)(const AVCodec *codec, int index);
  const char *(*avcodec_get_name_)(/*enum AVCodecID*/ int id);
  int (*av_codec_is_encoder_)(const AVCodec *codec);
  int (*av_codec_is_decoder_)(const AVCodec *codec);
  int (*avcodec_open2_)(AVCodecContext *avctx, const AVCodec *codec,
                        AVDictionary **options);
  int (*avcodec_receive_frame_)(AVCodecContext *avctx, AVFrame *frame);
  int (*avcodec_send_frame_)(AVCodecContext *avctx, const AVFrame *frame);
  int (*avcodec_receive_packet_)(AVCodecContext *avctx, AVPacket *avpkt);
  int (*avcodec_send_packet_)(AVCodecContext *avctx, const AVPacket *avpkt);
  void (*avcodec_register_all_)(void);

  // avformat
  unsigned (*avformat_version_)(void);

  void (*av_dump_format_)(AVFormatContext *ic, int index, const char *url, int is_output);
  AVInputFormat *(*av_find_input_format_)(const char *short_name);

  AVOutputFormat *(*av_guess_format_)(const char *short_name, const char *filename,
                                      const char *mime_type);

  int (*av_read_frame_)(AVFormatContext *s, AVPacket *pkt);
  int (*av_read_play_)(AVFormatContext *s);
  int (*av_read_pause_)(AVFormatContext *s);

  void (*av_register_all_)(void);
  int (*avformat_flush_)(AVFormatContext *s);
  int (*av_seek_frame_)(AVFormatContext *s, int stream_index, int64_t timestamp,
                        int flags);
  int (*av_write_frame_)(AVFormatContext *s, AVPacket *pkt);
  int (*av_interleaved_write_frame_)(AVFormatContext *s, AVPacket *pkt);

  int (*av_write_trailer_)(AVFormatContext *s);

  AVFormatContext *(*avformat_alloc_context_)(void);
  int (*avformat_alloc_output_context2_)(AVFormatContext **ctx, AVOutputFormat *oformat,
                                         const char *format_name, const char *filename);

  void (*avformat_free_context_)(AVFormatContext *s);

  void (*avformat_close_input_)(AVFormatContext **s);
  int (*avformat_find_stream_info_)(AVFormatContext *ic, AVDictionary **options);

  int (*avformat_network_init_)(void);
  int (*avformat_network_deinit_)(void);

  AVStream *(*avformat_new_stream_)(AVFormatContext *s, const AVCodec *c);

  int (*av_stream_add_side_data_)(AVStream *st, int type,
                              uint8_t *data, size_t size);
  uint8_t *(*av_stream_new_side_data_)(AVStream *stream,
                                   int type, int size);
  uint8_t *(*av_stream_get_side_data_)(const AVStream *stream,
                                   int type, int *size);


  int (*avformat_open_input_)(AVFormatContext **ps, const char *url, AVInputFormat *fmt,
                              AVDictionary **options);

  int (*avformat_write_header_)(AVFormatContext *s, AVDictionary **options);

  AVIOContext *(*avio_alloc_context_)(
    unsigned char *buffer, int buffer_size, int write_flag, void *opaque,
    int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),
    int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),
    int64_t (*seek)(void *opaque, int64_t offset, int whence));

  void (*avio_context_free_)(AVIOContext** s);

  int (*avio_close_)(AVIOContext *s);
  int (*avio_closep_)(AVIOContext **s);
  void (*avio_flush_)(AVIOContext *s);

  int (*avio_open2_)(AVIOContext **s, const char *url, int flags,
                     const AVIOInterruptCB *int_cb, AVDictionary **options);

  // avutil
  unsigned (*avutil_version_)(void);

  int (*av_samples_get_buffer_size_)(int *linesize, int nb_channels, int nb_samples,
                                     int /*enum AVSampleFormat*/ sample_fmt, int align);

  int64_t (*av_rescale_rnd_)(int64_t a, int64_t b, int64_t c,
                             int /*enum AVRounding*/ rnd);

  int (*av_samples_alloc_)(uint8_t **audio_data, int *linesize, int nb_channels,
                           int nb_samples, int /*enum AVSampleFormat*/ sample_fmt,
                           int /*(0 = default, 1 = no alignment)*/ align);

  int (*av_samples_alloc_array_and_samples_)(
    uint8_t ***audio_data, int *linesize, int nb_channels, int nb_samples,
    int /*enum AVSampleFormat*/ sample_fmt,
    int /*(0 = default, 1 = no alignment)*/ align);

  int (*av_opt_set_int_)(void *obj, const char *name, int64_t val, int search_flags);
  int (*av_opt_set_sample_fmt_)(void *obj, const char *name,
                                int /*enum AVSampleFormat*/ fmt, int search_flags);
  AVBufferRef *(*av_hwdevice_ctx_alloc_)(int /*enum AVHWDeviceType*/ type);
  int (*av_hwdevice_ctx_init_)(AVBufferRef *ref);
  AVBufferRef *(*av_hwframe_ctx_alloc_)(AVBufferRef *device_ctx);

  /*enum AVHWDeviceType*/ int (*av_hwdevice_find_type_by_name_)(const char *name);

  const char *(*av_hwdevice_get_type_name_)(int /*enum AVHWDeviceType*/ type);

  /*enum AVHWDeviceType*/ int (*av_hwdevice_iterate_types_)(int /*enum AVHWDeviceType*/ prev);

  int (*av_hwdevice_ctx_create_)(AVBufferRef **device_ctx,
                                 int /*enum AVHWDeviceType*/ type, const char *device,
                                 AVDictionary *opts, int flags);

  int (*av_hwframe_ctx_init_)(AVBufferRef *ref);
  int (*av_hwframe_get_buffer_)(AVBufferRef *hwframe_ctx, AVFrame *frame, int flags);
  int (*av_hwframe_transfer_data_)(AVFrame *dst, const AVFrame *src, int flags);

  int (*av_hwframe_transfer_get_formats_)(AVBufferRef *hwframe_ctx,
                                          int /*enum AVHWFrameTransferDirection*/ dir,
                                          int /*enum AVPixelFormat*/ **formats,
                                          int flags);

  void *(*av_hwdevice_hwconfig_alloc_)(AVBufferRef *device_ctx);

  AVHWFramesConstraints *(*av_hwdevice_get_hwframe_constraints_)(AVBufferRef *ref,
                                                                 const void *hwconfig);

  void (*av_hwframe_constraints_free_)(AVHWFramesConstraints **constraints);
  int (*av_hwframe_map_)(AVFrame *dst, const AVFrame *src, int flags);

  int (*av_dict_set_)(AVDictionary **pm, const char *key, const char *value, int flags);
  int (*av_dict_set_int_)(AVDictionary **pm, const char *key, int64_t value, int flags);
  AVDictionaryEntry *(*av_dict_get_)(const AVDictionary *m, const char *key,
                                     const AVDictionaryEntry *prev, int flags);
  int (*av_dict_count_)(const AVDictionary *m);
  void (*av_dict_free_)(AVDictionary **m);

  AVFrame *(*av_frame_alloc_)(void);
  void (*av_frame_free_)(AVFrame **frame);

  int (*av_frame_get_buffer_)(AVFrame *frame, int align);

  int (*av_frame_get_channels_)(const AVFrame *frame);
  void (*av_frame_set_channels_)(AVFrame *frame, int val);
  int64_t (*av_frame_get_pkt_duration_)(const AVFrame *frame);
  void (*av_frame_set_pkt_duration_)(AVFrame *frame, int64_t val);
  int64_t (*av_frame_get_pkt_pos_)(const AVFrame *frame);
  void (*av_frame_set_pkt_pos_)(AVFrame *frame, int64_t val);
  int (*av_frame_get_sample_rate_)(const AVFrame *frame);
  void (*av_frame_set_sample_rate_)(AVFrame *frame, int val);

  void (*av_free_)(void *ptr);
  void (*av_freep_)(void *ptr);

  int (*av_get_bytes_per_sample_)(/*enum AVSampleFormat*/ int sample_fmt);

  int (*av_image_copy_to_buffer_)(uint8_t *dst, int dst_size,
                                  const uint8_t *const src_data[4],
                                  const int src_linesize[4], /*enum AVPixelFormat*/ int pix_fmt,
                                  int width, int height, int align);

  int (*av_image_fill_arrays_)(uint8_t *dst_data[4], int dst_linesize[4],
                               const uint8_t *src, /*enum AVPixelFormat*/ int pix_fmt, int width,
                               int height, int align);

  int (*av_image_get_buffer_size_)(/*enum AVPixelFormat*/ int pix_fmt, int width, int height,
                                   int align);

  void (*av_log_default_callback_)(void *avcl, int level, const char *fmt, va_list vl);

  void (*av_log_set_callback_)(void (*callback)(void *, int, const char *, va_list));

  void (*av_log_set_level_)(int level);

  void *(*av_malloc_)(size_t size);
  char *(*av_strdup_)(const char *s);

  AVBufferRef *(*av_buffer_create_)(uint8_t *data, int size,
                                    void (*free)(void *opaque, uint8_t *data),
                                    void *opaque, int flags);

  int (*av_buffer_is_writable_)(const AVBufferRef *buf);
  void *(*av_buffer_get_opaque_)(const AVBufferRef *buf);
  int (*av_buffer_get_ref_count_)(const AVBufferRef *buf);
  int (*av_buffer_make_writable_)(AVBufferRef **buf);

  int (*av_buffer_realloc_)(AVBufferRef **pbuf, int size);

  void (*av_buffer_unref_)(AVBufferRef **buf);

  AVBufferRef *(*av_buffer_ref_)(AVBufferRef *buf);

  int (*av_pix_fmt_count_planes_)(/*enum AVPixelFormat*/ int pix_fmt);
  const AVPixFmtDescriptor* (*av_pix_fmt_desc_get_)(int /*enum AVPixelFormat*/ pix_fmt);

  int (*av_sample_fmt_is_planar_)(/*enum AVSampleFormat*/ int sample_fmt);

  int (*av_samples_set_silence_)(uint8_t **audio_data, int offset, int nb_samples,
                                 int nb_channels, /*enum AVSampleFormat*/ int sample_fmt);

  int (*av_strerror_)(int errnum, char *errbuf, size_t errbuf_size);

  uint64_t (*av_get_channel_layout_)(const char *name);
  int (*av_get_channel_layout_nb_channels_)(uint64_t channel_layout);
  int64_t (*av_get_default_channel_layout_)(int nb_channels);

  int (*av_get_channel_layout_channel_index_)(uint64_t channel_layout, uint64_t channel);

  uint64_t (*av_channel_layout_extract_channel_)(uint64_t channel_layout, int index);

  const char *(*av_get_channel_name_)(uint64_t channel);

  const char *(*av_get_channel_description_)(uint64_t channel);
  int (*av_get_standard_channel_layout_)(unsigned index, uint64_t *layout,
                                         const char **name);

  // swscale
  unsigned (*swscale_version_)(void);

  void (*sws_freeContext_)(struct SwsContext *swsContext);

  struct SwsContext *(*sws_getContext_)(int srcW, int srcH, /*enum AVPixelFormat*/ int srcFormat,
                                        int dstW, int dstH, /*enum AVPixelFormat*/ int dstFormat,
                                        int flags, SwsFilter *srcFilter,
                                        SwsFilter *dstFilter, const double *param);

  int (*sws_scale_)(struct SwsContext *c, const uint8_t *const srcSlice[],
                    const int srcStride[], int srcSliceY, int srcSliceH,
                    uint8_t *const dst[], const int dstStride[]);

  // swresample
  unsigned (*swresample_version_)(void);

  struct SwrContext *(*swr_alloc_)();
  int (*swr_init_)(struct SwrContext *s);
  int (*swr_is_initialized_)(struct SwrContext *s);
  void (*swr_free_)(struct SwrContext **s);
  void (*swr_close_)(struct SwrContext *s);
  int (*swr_convert_)(struct SwrContext *s, uint8_t **out, int out_count,
                      const uint8_t **in, int in_count);
  int64_t (*swr_get_delay_)(struct SwrContext *s, int64_t base);

  // avdevice
  unsigned (*avdevice_version_)(void);
  void (*avdevice_register_all_)(void);

 private:
  std::shared_ptr<cmf::IDynamicModulesLoader> modules_loader_;
  std::shared_ptr<IAvcModuleLoadHandler> load_handler_;
  std::string modules_path_;

  void *avcodec_handle_;
  void *avformat_handle_;
  void *avutil_handle_;
  void *avdevice_handle_;
  void *swscale_handle_;
  void *swresample_handle_;

  std::string avcodec_module_name_;
  std::string avformat_module_name_;
  std::string avutil_module_name_;
  std::string avdevice_module_name_;
  std::string swscale_module_name_;
  std::string swresample_module_name_;

  std::shared_ptr<IAvcModuleDataWrapper> data_wrapper_;
  std::shared_ptr<IAvcVideoPixelFormatConverter> video_pixel_format_converter_;
};

}  // namespace detail
}  // namespace avc

#endif  // AVC_MODULE_PROVIDER_H
