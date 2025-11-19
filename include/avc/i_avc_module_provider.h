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

#ifndef I_AVC_MODULE_PROVIDER_H
#define I_AVC_MODULE_PROVIDER_H

#include <cstdarg>
#include <cstdint>
#include <memory>
#include <vector>
#include <string>

#include <avc/i_avc_module_data_wrapper.h>
#include <avc/i_avc_video_pixel_format_converter.h>
#include <media/media_timebase.h>
#include <media/video_pixel_format.h>

namespace avc {

enum AvcCompliance {
  kAvcCompliance_VeryStrict = 2,
  kAvcCompliance_Strict = 1,
  kAvcCompliance_Normal = 0,
  kAvcCompliance_Unofficial = -1,
  kAvcCompliance_Experimental = -2
};

struct AvcModuleVersion {
  unsigned int avcodec_version_ = 0;
  unsigned int avutil_version_ = 0;
  unsigned int avformat_version_ = 0;
  unsigned int avdevice_version_ = 0;
  unsigned int swscale_version_ = 0;
  unsigned int swresample_version_ = 0;

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

/// \brief  IAvcModuleProvider interface declaration
struct IAvcModuleProvider {
  virtual ~IAvcModuleProvider() = default;

  virtual bool IsAvCodecLoaded() const = 0;
  virtual bool IsAvFormatLoaded() const = 0;
  virtual bool IsAvUtilLoaded() const = 0;
  virtual bool IsAvDeviceLoaded() const = 0;
  virtual bool IsSwScaleLoaded() const = 0;
  virtual bool IsSwResampleLoaded() const = 0;

  virtual const std::string& GetAvCodecModulePath() const = 0;
  virtual const std::string& GetAvFormatModulePath() const = 0;
  virtual const std::string& GetAvUtilModulePath() const = 0;
  virtual const std::string& GetAvDeviceModulePath() const = 0;
  virtual const std::string& GetSwScaleModulePath() const = 0;
  virtual const std::string& GetSwResampleModulePath() const = 0;

  virtual int GetLibrariesCompatibilityScore() const = 0;

  // avcodec
  virtual unsigned avcodec_version() = 0;

  virtual int avcodec_close(AVCodecContext* avctx) = 0;

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
  virtual void av_packet_rescale_ts(AVPacket* pkt, cmf::MediaTimeBase tb_src, cmf::MediaTimeBase tb_dst) = 0;

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

  virtual const char *avcodec_configuration(void) = 0;
  virtual const char *avcodec_license(void) = 0;
  virtual const AVClass *avcodec_get_class(void) = 0;
  virtual void avsubtitle_free(AVSubtitle *sub) = 0;
  virtual int avcodec_align_dimensions(AVCodecContext *s, int *width, int *height) = 0;
  virtual int avcodec_align_dimensions2(AVCodecContext *s, int *width, int *height, int linesize_align[8]) = 0;
  virtual int avcodec_enum_to_chroma_pos(int *xpos, int *ypos, int /*enum AVChromaLocation*/ pos) = 0;
  virtual int avcodec_chroma_pos_to_enum(int *xpos, int *ypos, int /*enum AVChromaLocation*/ pos) = 0;
  virtual int avcodec_decode_subtitle2(AVCodecContext *avctx, AVSubtitle *sub, int *got_sub_ptr, const AVPacket *avpkt) = 0;
  virtual int avcodec_get_hw_frames_parameters(AVCodecContext *avctx, AVBufferRef *device_ref, const char *hw_pix_fmt, AVBufferRef **out_frames_ref) = 0;
  virtual AVCodecParserContext *av_parser_init(int /*enum AVCodecID*/ codec_id) = 0;
  virtual const AVCodecParser *av_parser_iterate(void **opaque) = 0;
  virtual int av_parser_parse2(AVCodecParserContext *s, AVCodecContext *avctx, uint8_t **poutbuf, int *poutbuf_size, const uint8_t *buf, int buf_size, int64_t pts, int64_t dts, int64_t pos) = 0;
  virtual void av_parser_close(AVCodecParserContext *s) = 0;
  virtual int avcodec_encode_subtitle(AVCodecContext *avctx, uint8_t *buf, int buf_size, const AVSubtitle *sub) = 0;
  virtual unsigned int avcodec_pix_fmt_to_codec_tag(const AVPixFmtDescriptor *pix_fmt) = 0;
  virtual int /*enum AVPixelFormat*/ avcodec_find_best_pix_fmt_of_list(const int /*enum AVPixelFormat*/ *pix_fmt_list, int /*enum AVPixelFormat*/ src_pix_fmt, int has_alpha, int *loss_ptr) = 0;
  virtual int /*enum AVPixelFormat*/ avcodec_default_get_format(struct AVCodecContext *s, const int /*enum AVPixelFormat*/ *fmt) = 0;
  virtual int avcodec_fill_audio_frame(AVFrame *frame, int nb_channels, int /*enum AVSampleFormat*/ sample_fmt, const uint8_t *buf, int buf_size, int align) = 0;
  virtual int av_get_audio_frame_duration(AVCodecContext *avctx, int frame_bytes) = 0;
  virtual void av_fast_padded_malloc(void *ptr, unsigned int *size, size_t min_size) = 0;
  virtual void av_fast_padded_mallocz(void *ptr, unsigned int *size, size_t min_size) = 0;
  virtual int avcodec_is_open(AVCodecContext *s) = 0;

  // avformat functions
  virtual unsigned avformat_version() = 0;

  virtual cmf::MediaTimeBase av_guess_sample_aspect_ratio(AVFormatContext* ctx, AVStream* stream, AVFrame* frame) = 0;
  virtual cmf::MediaTimeBase av_guess_frame_rate(AVFormatContext* ctx, AVStream* stream, AVFrame* frame) = 0;

  virtual void av_dump_format(AVFormatContext *ic, int index, const char *url,
                              int is_output) = 0;
  virtual AVInputFormat *av_find_input_format(const char *short_name) = 0;

  virtual AVOutputFormat *av_guess_format(const char *short_name, const char *filename,
                                          const char *mime_type) = 0;

  virtual int av_guess_codec(AVOutputFormat *fmt, const char *short_name, const char *filename,
                            const char *mime_type, int /*enum AVMediaType*/ type) = 0;

  virtual int av_find_best_stream(AVFormatContext *ic, int /*enum AVMediaType*/ type, int wanted_stream_nb, int related_stream, const AVCodec **decoder_ret, int flags) = 0;

  virtual int avformat_init_output(AVFormatContext *s, AVDictionary **options) = 0;

  virtual int av_write_uncoded_frame(AVFormatContext *s, int stream_index, AVFrame *frame) = 0;

  virtual int av_interleaved_write_uncoded_frame(AVFormatContext *s, int stream_index, AVFrame *frame) = 0;

  virtual int av_write_uncoded_frame_query(AVFormatContext *s, int stream_index) = 0;

  virtual int av_get_output_timestamp(AVFormatContext *s, int stream, int64_t *dts, int64_t *wall) = 0;

  virtual int av_index_search_timestamp(AVStream *st, int64_t timestamp, int flags) = 0;

  virtual int avformat_index_get_entries_count(AVStream *st) = 0;

  virtual AVIndexEntry *avformat_index_get_entry(AVStream *st, int idx) = 0;

  virtual AVIndexEntry *avformat_index_get_entry_from_timestamp(AVStream *st, int64_t wanted_timestamp, int flags) = 0;

  virtual int av_add_index_entry(AVStream *st, int64_t pos, int64_t timestamp, int size, int distance, int flags) = 0;

  virtual void av_url_split(char *proto, int proto_size, char *authorization, int authorization_size, char *hostname, int hostname_size, int *port_ptr, char *path, int path_size, const char *url) = 0;

  virtual int av_sdp_create(AVFormatContext *ac[], int n_files, char *buf, int size) = 0;

  virtual int av_match_ext(const char *filename, const char *extensions) = 0;

  virtual int avformat_query_codec(const AVOutputFormat *ofmt, int /*enum AVCodecID*/ codec_id, int std_compliance) = 0;

  virtual const char *avformat_get_riff_video_tags(void) = 0;

  virtual const char *avformat_get_riff_audio_tags(void) = 0;

  virtual const char *avformat_get_mov_video_tags(void) = 0;

  virtual const char *avformat_get_mov_audio_tags(void) = 0;

  virtual AVRational av_stream_get_codec_timebase(const AVStream *st) = 0;

  virtual int av_read_frame(AVFormatContext *s, AVPacket *pkt) = 0;
  virtual int av_read_play(AVFormatContext *s) = 0;
  virtual int av_read_pause(AVFormatContext *s) = 0;

  virtual void av_register_all(void) = 0;

  virtual int avformat_seek_file(AVFormatContext* s, int stream_index,
    int64_t min_ts, int64_t ts, int64_t max_ts, int flags) = 0;

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

  virtual int64_t av_rescale_q_rnd(int64_t a, AVRational bq, AVRational cq, int /*enum AVRounding*/ rnd) = 0;

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

  virtual int av_frame_ref(AVFrame* dst, const AVFrame* src) = 0;
  virtual int av_frame_replace(AVFrame* dst, const AVFrame* src) = 0;
  virtual AVFrame* av_frame_clone(const AVFrame* src) = 0;
  virtual void av_frame_unref(AVFrame* frame) = 0;
  virtual void av_frame_move_ref(AVFrame* dst, AVFrame* src) = 0;
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

  // new api
  virtual int av_channel_layout_from_mask(AVChannelLayout* channel_layout, uint64_t mask) = 0;
  virtual int av_channel_layout_from_string(AVChannelLayout* channel_layout,
    const char* str) = 0;
  virtual void av_channel_layout_default(AVChannelLayout* ch_layout, int nb_channels) = 0;
  virtual const AVChannelLayout* av_channel_layout_standard(void** opaque) = 0;
  virtual void av_channel_layout_uninit(AVChannelLayout* channel_layout) = 0;
  virtual int av_channel_layout_copy(AVChannelLayout* dst, const AVChannelLayout* src) = 0;
  virtual int av_channel_layout_describe(const AVChannelLayout* channel_layout,
    char* buf, size_t buf_size) = 0;

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

  virtual struct SwrContext* swr_alloc_set_opts(struct SwrContext* s,
    int64_t out_ch_layout, int /*enum AVSampleFormat*/ out_sample_fmt, int out_sample_rate,
    int64_t  in_ch_layout, int /*enum AVSampleFormat*/  in_sample_fmt, int  in_sample_rate,
    int log_offset, void* log_ctx) = 0;

  virtual int swr_alloc_set_opts2(struct SwrContext** ps,
    AVChannelLayout* out_ch_layout, int /*enum AVSampleFormat*/ out_sample_fmt, int out_sample_rate,
    AVChannelLayout* in_ch_layout, int /*enum AVSampleFormat*/  in_sample_fmt, int  in_sample_rate,
    int log_offset, void* log_ctx) = 0;
  virtual int64_t swr_next_pts(struct SwrContext* s, int64_t pts) = 0;
  virtual int swr_set_compensation(struct SwrContext* s, int sample_delta, int compensation_distance) = 0;
  virtual int swr_set_channel_mapping(struct SwrContext* s, const int* channel_map) = 0;
  virtual int swr_drop_output(struct SwrContext* s, int count) = 0;
  virtual int swr_inject_silence(struct SwrContext* s, int count) = 0;
  virtual int swr_get_out_samples(struct SwrContext* s, int in_samples) = 0;
  virtual int swr_convert_frame(SwrContext* swr,
    AVFrame* output, const AVFrame* input) = 0;

  // avdevice
  virtual unsigned avdevice_version() = 0;
  virtual void avdevice_register_all() = 0;


  // wrap data structures
  virtual std::shared_ptr<IAvcModuleDataWrapper> d() const = 0;

  virtual std::shared_ptr<IAvcVideoPixelFormatConverter> GetVideoPixelFormatConverter() = 0;
};

}  // namespace avc

#endif  // I_AVC_MODULE_PROVIDER_H
