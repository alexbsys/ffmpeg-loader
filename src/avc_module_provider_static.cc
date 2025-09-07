
#include "avc_module_provider.h"

#ifdef AVC_LIBRARIES_STATIC_LINK
extern "C" {
#include <libavutil/avutil.h>
#include <libavdevice/avdevice.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavutil/channel_layout.h>
#include <libswresample/swresample.h>
}//extern "C"
#endif //AVC_LIBRARIES_STATIC_LINK


#ifdef AVC_LIBRARIES_STATIC_LINK

AVBufferRef *av_buffer_create_wrapper(uint8_t *data, size_t size,
                                      void (*free)(void *opaque, uint8_t *data),
                                      void *opaque, int flags) {
  return ::av_buffer_create(data, static_cast<int>(size), free, opaque, flags);
}

int av_buffer_realloc_wrapper(AVBufferRef **buf, size_t size) {
  return ::av_buffer_realloc(buf, static_cast<int>(size));
}

int av_buffer_realloc_wrapper2(AVBufferRef **buf, int size) {
  return ::av_buffer_realloc(buf, static_cast<size_t>(size));
}

void AvcDataProvidersGlobalInit();

namespace cmf {
namespace detail {

template<typename T1, typename T2>
void Assign(T1& variable, T2* value) {
  variable = reinterpret_cast<T1>(value);
}

#define AVC_STATIC_HAS_AVFRAME_GETSET_FNS   0
#define AVC_STATIC_HAS_AV_GET_CHANNEL_FNS   1
#define AVC_STATIC_HAS_AVCODEC_REGISTER_ALL 0

#define AVC_STATIC_HAS_AVCODEC_ENCODE_VIDEO2  0
#define AVC_STATIC_HAS_AV_GET_CODEC_TAG_STRING  0
#define AVC_STATIC_HAS_HWACCEL_FNS          0
#define AVC_STATIC_HAS_LOCKMGR_REGISTER_FN  0
#define AVC_STATIC_HAS_AV_HWDEVICE_CTX  0

void AvcModuleProvider::LoadStatically() {
  AvcDataProvidersGlobalInit();

  avcodec_version_ = &::avcodec_version;

#if AVC_STATIC_HAS_AVCODEC_ENCODE_VIDEO2
  Assign(avcodec_encode_video2_, &::avcodec_encode_video2);
#else 
  avcodec_encode_video2_ = nullptr;    // &::avcodec_encode_video2;
#endif

#if AVC_STATIC_HAS_AV_GET_CODEC_TAG_STRING
  Assign(av_get_codec_tag_string_, &::av_get_codec_tag_string);
#else
  av_get_codec_tag_string_ = nullptr;  // &::av_get_codec_tag_string;
#endif

  Assign(avcodec_parameters_copy_, &::avcodec_parameters_copy);
  Assign(avcodec_parameters_from_context_, &::avcodec_parameters_from_context);
  Assign(avcodec_parameters_free_, &::avcodec_parameters_free);
  Assign(avcodec_parameters_alloc_, &::avcodec_parameters_alloc);
  Assign(avcodec_parameters_to_context_, &::avcodec_parameters_to_context);
  Assign(av_packet_alloc_, &::av_packet_alloc);
  Assign(av_packet_clone_, &::av_packet_clone);
  Assign(av_packet_free_, &::av_packet_free);
  Assign(av_init_packet_, &::av_init_packet);
  Assign(av_new_packet_, &::av_new_packet);
  Assign(av_packet_ref_, &::av_packet_ref);
  Assign(av_packet_unref_, &::av_packet_unref);
  Assign(avcodec_alloc_context3_, &::avcodec_alloc_context3);
  Assign(avcodec_free_context_, &::avcodec_free_context);

#if AVC_STATIC_HAS_HWACCEL_FNS
  Assign(av_register_hwaccel_, &::av_register_hwaccel);
  Assign(av_hwaccel_next_, &::av_hwaccel_next);
#else //AVC_STATIC_HAS_HWACCEL_FNS
  av_register_hwaccel_ = nullptr;  // &::av_register_hwaccel;
  av_hwaccel_next_ = nullptr;      // &::av_hwaccel_next;
#endif //AVC_STATIC_HAS_HWACCEL_FNS

#if AVC_STATIC_HAS_LOCKMGR_REGISTER_FN
  Assign(av_lockmgr_register_, &::av_lockmgr_register);
#else //AVC_STATIC_HAS_LOCKMGR_REGISTER_FN
  av_lockmgr_register_ = nullptr;  // &::av_lockmgr_register;
#endif //AVC_STATIC_HAS_LOCKMGR_REGISTER_FN

  avcodec_find_decoder_ =
    reinterpret_cast<AVCodec *(*)(int id)>(&::avcodec_find_decoder);
  avcodec_find_decoder_by_name_ =
    reinterpret_cast<AVCodec *(*)(const char *name)>(&::avcodec_find_decoder_by_name);
  avcodec_find_encoder_ =
    reinterpret_cast<AVCodec *(*)(int id)>(&::avcodec_find_encoder);
  avcodec_find_encoder_by_name_ =
    reinterpret_cast<AVCodec *(*)(const char *name)>(&::avcodec_find_encoder_by_name);
  Assign(avcodec_flush_buffers_, &::avcodec_flush_buffers);
  avcodec_get_name_ = reinterpret_cast<const char *(*)(int id)>(&::avcodec_get_name);
  Assign(av_codec_is_encoder_, &::av_codec_is_encoder);
  Assign(av_codec_is_decoder_, &::av_codec_is_decoder);
  Assign(avcodec_open2_, &::avcodec_open2);
  Assign(avcodec_receive_frame_, &::avcodec_receive_frame);
  Assign(avcodec_send_frame_, &::avcodec_send_frame);
  Assign(avcodec_receive_packet_, &::avcodec_receive_packet);
  Assign(avcodec_send_packet_, &::avcodec_send_packet);
  Assign(av_codec_iterate_, &::av_codec_iterate);
  Assign(avcodec_get_hw_config_, &::avcodec_get_hw_config);

  avformat_version_ = &::avformat_version;
  Assign(av_dump_format_, &::av_dump_format);
  av_find_input_format_ =
    reinterpret_cast<AVInputFormat *(*)(const char *short_name)>(&::av_find_input_format);
  av_guess_format_ =
    reinterpret_cast<AVOutputFormat *(*)(const char *short_name, const char *filename,
                                          const char *mime_type)>(&::av_guess_format);
  Assign(av_read_frame_, &::av_read_frame);
  Assign(av_read_play_, &::av_read_play);
  Assign(av_read_pause_, &::av_read_pause);

#if AVC_STATIC_HAS_AVCODEC_REGISTER_ALL
  Assign(av_register_all_, &::av_register_all);
  Assign(avcodec_register_all_, &::avcodec_register_all);
#else //AVC_STATIC_HAS_AVCODEC_REGISTER_ALL
  av_register_all_ = nullptr;  // &::av_register_all;
  avcodec_register_all_ = nullptr;  //&::avcodec_register_all;
#endif //AVC_STATIC_HAS_AVCODEC_REGISTER_ALL

  Assign(avformat_flush_, &::avformat_flush);
  Assign(av_seek_frame_, &::av_seek_frame);
  Assign(av_write_frame_, &::av_write_frame);
  Assign(av_interleaved_write_frame_, &::av_interleaved_write_frame);
  Assign(av_write_trailer_, &::av_write_trailer);
  Assign(avformat_alloc_context_, &::avformat_alloc_context);
  Assign(avformat_alloc_output_context2_, &::avformat_alloc_output_context2);
  Assign(avformat_free_context_, &::avformat_free_context);
  Assign(avformat_close_input_, &::avformat_close_input);
  Assign(avformat_find_stream_info_, &::avformat_find_stream_info);
  Assign(avformat_network_init_, &::avformat_network_init);
  Assign(avformat_network_deinit_, &::avformat_network_deinit);
  Assign(avformat_new_stream_, &::avformat_new_stream);
  avformat_open_input_ =
    reinterpret_cast<int (*)(AVFormatContext * *ps, const char *url, AVInputFormat *fmt,
                             AVDictionary **options)>(&::avformat_open_input);
  Assign(avformat_write_header_, &::avformat_write_header);
  Assign(avio_alloc_context_, &::avio_alloc_context);
  Assign(avio_context_free_, &::avio_context_free);
  Assign(avio_close_, &::avio_close);
  Assign(avio_closep_, &::avio_closep);
  Assign(avio_flush_, &::avio_flush);
  Assign(avio_open2_, &::avio_open2);

#if AVC_STATIC_HAS_AV_HWDEVICE_CTX
  av_hwdevice_ctx_alloc_ =
    reinterpret_cast<AVBufferRef *(*)(int type)>(
      &::av_hwdevice_ctx_alloc);
  Assign(av_hwdevice_ctx_init_, &::av_hwdevice_ctx_init);
  Assign(av_hwframe_ctx_alloc_, &::av_hwframe_ctx_alloc);
  av_hwdevice_find_type_by_name_ = reinterpret_cast</*enum AVHWDeviceType*/ int (*)(const char *name)>(&::av_hwdevice_find_type_by_name);

  av_hwdevice_get_type_name_ =
    reinterpret_cast<const char *(*)(int /*enum AVHWDeviceType*/ type)>(
      &::av_hwdevice_get_type_name);

  av_hwdevice_iterate_types_ =
    reinterpret_cast</*enum AVHWDeviceType*/ int (*)(int /*enum AVHWDeviceType*/ prev)>(
      &::av_hwdevice_iterate_types);

  av_hwdevice_ctx_create_ =
    reinterpret_cast<int (*)(AVBufferRef * *device_ctx, int /*enum AVHWDeviceType*/ type,
                             const char *device, AVDictionary *opts, int flags)>(
      &::av_hwdevice_ctx_create);
#endif

  Assign(av_hwframe_ctx_init_, &::av_hwframe_ctx_init);
  Assign(av_hwframe_get_buffer_, &::av_hwframe_get_buffer);
  Assign(av_hwframe_transfer_data_, &::av_hwframe_transfer_data);

  av_hwframe_transfer_get_formats_ = reinterpret_cast<int (*)(
    AVBufferRef * hwframe_ctx, int dir,
    int **formats, int flags)>(&::av_hwframe_transfer_get_formats);

  Assign(av_hwdevice_hwconfig_alloc_, &::av_hwdevice_hwconfig_alloc);
  Assign(av_hwdevice_get_hwframe_constraints_, &::av_hwdevice_get_hwframe_constraints);
  Assign(av_hwframe_constraints_free_, &::av_hwframe_constraints_free);
  Assign(av_hwframe_map_, &::av_hwframe_map);

  Assign(avutil_version_, &::avutil_version);

  Assign(av_dict_set_, &::av_dict_set);
  Assign(av_dict_set_int_, &::av_dict_set_int);
  Assign(av_dict_get_, &::av_dict_get);
  Assign(av_dict_count_, &::av_dict_count);
  Assign(av_dict_free_, &::av_dict_free);
  Assign(av_frame_alloc_, &::av_frame_alloc);
  Assign(av_frame_free_, &::av_frame_free);
  Assign(av_frame_get_buffer_, &::av_frame_get_buffer);

#if AVC_STATIC_HAS_AVFRAME_GETSET_FNS
  Assign(av_frame_get_channels_, &::av_frame_get_channels);
  Assign(av_frame_set_channels_, &::av_frame_set_channels);
  Assign(av_frame_get_pkt_duration_, &::av_frame_get_pkt_duration);
  Assign(av_frame_set_pkt_duration_, &::av_frame_set_pkt_duration);
  Assign(av_frame_get_pkt_pos_, &::av_frame_get_pkt_pos);
  Assign(av_frame_set_pkt_pos_, &::av_frame_set_pkt_pos);
  Assign(av_frame_get_sample_rate_, &::av_frame_get_sample_rate);
  Assign(av_frame_set_sample_rate_, &::av_frame_set_sample_rate);

#else //AVC_STATIC_HAS_AVFRAME_GETSET_FNS
  av_frame_get_channels_ = nullptr;      //&::av_frame_get_channels;
  av_frame_set_channels_ = nullptr;      //&::av_frame_set_channels;
  av_frame_get_pkt_duration_ = nullptr;  //&::av_frame_get_pkt_duration;
  av_frame_set_pkt_duration_ = nullptr;  // &::av_frame_set_pkt_duration;
  av_frame_get_pkt_pos_ = nullptr;       // &::av_frame_get_pkt_pos;
  av_frame_set_pkt_pos_ = nullptr;       // &::av_frame_set_pkt_pos;
  av_frame_get_sample_rate_ = nullptr;   // &::av_frame_get_sample_rate;
  av_frame_set_sample_rate_ = nullptr;   // &::av_frame_set_sample_rate;
#endif //AVC_STATIC_HAS_AVFRAME_GETSET_FNS
  av_free_ = &::av_free;
  av_freep_ = &::av_freep;

  av_get_bytes_per_sample_ = reinterpret_cast<int (*)(int)>(&::av_get_bytes_per_sample);
  av_image_copy_to_buffer_ = reinterpret_cast<int (*)(uint8_t *dst, int dst_size,
                                                      const uint8_t *const src_data[4],
                                                      const int src_linesize[4], int pix_fmt,
                                                      int width, int height, int align)>(&::av_image_copy_to_buffer);
  av_image_fill_arrays_ = reinterpret_cast<int (*)(uint8_t *dst_data[4], int dst_linesize[4],
                                                   const uint8_t *src, int pix_fmt, int width,
                                                   int height, int align)>(&::av_image_fill_arrays);
  av_image_get_buffer_size_ = reinterpret_cast<int (*)(int pix_fmt, int width, int height,
                                                       int align)>(&::av_image_get_buffer_size);
  av_log_default_callback_ = &::av_log_default_callback;
  av_log_set_callback_ = &::av_log_set_callback;
  av_log_set_level_ = &::av_log_set_level;
  av_malloc_ = &::av_malloc;
  av_pix_fmt_count_planes_ = reinterpret_cast<int (*)(int pix_fmt)>(&::av_pix_fmt_count_planes);
  av_pix_fmt_desc_get_ = reinterpret_cast<const AVPixFmtDescriptor*(*)(int pix_fmt)>(&::av_pix_fmt_desc_get);
  av_sample_fmt_is_planar_ = reinterpret_cast<int (*)(int sample_fmt)>(&::av_sample_fmt_is_planar);
  av_samples_set_silence_ = reinterpret_cast<int (*)(uint8_t **audio_data, int offset, int nb_samples,
                                                     int nb_channels, int sample_fmt)>(&::av_samples_set_silence);

  av_strerror_ = &::av_strerror;

  av_buffer_create_ = reinterpret_cast<AVBufferRef *(*)(uint8_t *data, int size,
                                                         void (*free)(void *opaque, uint8_t *data),
                                                         void *opaque, int flags)>(&av_buffer_create_wrapper);
  Assign(av_buffer_is_writable_, &::av_buffer_is_writable);
  Assign(av_buffer_get_opaque_, &::av_buffer_get_opaque);
  Assign(av_buffer_get_ref_count_, &::av_buffer_get_ref_count);
  Assign(av_buffer_make_writable_, &::av_buffer_make_writable);
  Assign(av_buffer_realloc_, &av_buffer_realloc_wrapper2);

  Assign(av_buffer_unref_, &::av_buffer_unref);
  Assign(av_buffer_ref_, &::av_buffer_ref);

#if AVC_STATIC_HAS_AV_GET_CHANNEL_FNS
  Assign(av_get_channel_layout_, &::av_get_channel_layout);
  Assign(av_get_channel_layout_nb_channels_, &::av_get_channel_layout_nb_channels);
  Assign(av_get_default_channel_layout_, &::av_get_default_channel_layout);
  Assign(av_get_channel_layout_channel_index_, &::av_get_channel_layout_channel_index);
  Assign(av_channel_layout_extract_channel_, &::av_channel_layout_extract_channel);
  Assign(av_get_channel_name_, &::av_get_channel_name);
  Assign(av_get_channel_description_, &::av_get_channel_description);
  Assign(av_get_standard_channel_layout_, &::av_get_standard_channel_layout);
#else //AVC_STATIC_HAS_AV_GET_CHANNEL_FNS
  av_get_channel_layout_ = nullptr;              // &::av_get_channel_layout;
  av_get_channel_layout_nb_channels_ = nullptr;  // &::av_get_channel_layout_nb_channels;
  av_get_default_channel_layout_ = &::av_get_default_channel_layout;
  av_get_channel_layout_channel_index_ =
    nullptr;  // &::av_get_channel_layout_channel_index;
  av_channel_layout_extract_channel_ = nullptr;  // &::av_channel_layout_extract_channel;
  av_get_channel_name_ = nullptr;                // &::av_get_channel_name;
  av_get_channel_description_ = nullptr;         // &::av_get_channel_description;
  av_get_standard_channel_layout_ = nullptr;     // &::av_get_standard_channel_layout;
#endif //AVC_STATIC_HAS_AV_GET_CHANNEL_FNS

  swscale_version_ = &::swscale_version;

  Assign(sws_freeContext_, &::sws_freeContext);
  sws_getContext_ = reinterpret_cast<struct SwsContext *(*)(int srcW, int srcH, /*enum AVPixelFormat*/ int srcFormat,
                                                             int dstW, int dstH, /*enum AVPixelFormat*/ int dstFormat,
                                                             int flags, SwsFilter *srcFilter,
                                                             SwsFilter *dstFilter, const double *param)>(&::sws_getContext);
  Assign(sws_scale_, &::sws_scale);
  avdevice_version_ = &::avdevice_version;
  avdevice_register_all_ = &::avdevice_register_all;

  av_strdup_ = &::av_strdup;

  // swresample
  swresample_version_ = &::swresample_version;
  Assign(swr_alloc_, &::swr_alloc);
  Assign(swr_init_, &::swr_init);
  Assign(swr_is_initialized_, &::swr_is_initialized);
  Assign(swr_free_, &::swr_free);
  Assign(swr_close_, &::swr_close);
  Assign(swr_convert_, &::swr_convert);
  Assign(swr_get_delay_, &::swr_get_delay);


  //new
  Assign(av_samples_get_buffer_size_, &::av_samples_get_buffer_size);
  Assign(av_rescale_rnd_, &::av_rescale_rnd);
  Assign(av_samples_alloc_, &::av_samples_alloc);
  Assign(av_samples_alloc_array_and_samples_, &::av_samples_alloc_array_and_samples);
  Assign(av_opt_set_int_, &::av_opt_set_int);
  Assign(av_opt_set_sample_fmt_, &::av_opt_set_sample_fmt);

  Assign(av_stream_add_side_data_, &::av_stream_add_side_data);
  Assign(av_stream_new_side_data_, &::av_stream_new_side_data);
  Assign(av_stream_get_side_data_, &::av_stream_get_side_data);


  void *vtrue = (void *)1;
  avcodec_handle_ = vtrue;
  avdevice_handle_ = vtrue;
  avformat_handle_ = vtrue;
  swscale_handle_ = vtrue;
  avutil_handle_ = vtrue;
  swresample_handle_ = vtrue;
}

}//namespace detail
}//namespace cmf

#endif /*AVC_LIBRARIES_STATIC_LINK*/

