
#include "avc_module_provider.h"
#include "avc_pixel_format_converter.h"
#include "avc_default_libraries_names.hpp"
#include "avc_dynamic_modules_loader.h"
#include "i_avc_module_data_wrapper_factory.h"

#include <avc/ffmpeg-loader.h>

#ifndef FFMPEG_LOADER_DLL
#define FFMPEG_LOADER_DLL 0
#endif //FFMPEG_LOADER_DLL

#if FFMPEG_LOADER_DLL
#include <tools/dynamic_export.h>
#else //
#define API_EXPORT
#endif //FFMPEG_LOADER_DLL

#if !defined(AVC_LIBRARIES_STATIC_LINK) || AVC_LIBRARIES_STATIC_LINK==0
#include "dynamic_loader.hpp"
#endif //AVC_LIBRARIES_STATIC_LINK

std::vector<std::shared_ptr<avc::IAvcModuleDataWrapperFactory> > g_ffmpeg_data_wrappers;

// this function is provided by auto-generated loaders for specific FFmpeg versions
void AvcDataProvidersGlobalInit();

namespace avc {

std::shared_ptr<IAvcModuleProvider> API_EXPORT CreateAvcModuleProvider(
    std::shared_ptr<cmf::IDynamicModulesLoader> modules_loader,
    std::shared_ptr<avc::IAvcModuleLoadHandler> load_handler,
    const std::string& modules_path,
    bool auto_load) {
  if (!modules_loader)
    modules_loader = CreateAvcDynamicModulesLoader();

  auto module_provider = std::make_shared<detail::AvcModuleProvider>(modules_loader, load_handler, modules_path);
  if (auto_load) {
    module_provider->Load();
  }
  return module_provider;
}

std::shared_ptr<IAvcModuleProvider> API_EXPORT CreateAvcModuleProvider2(
  std::shared_ptr<cmf::IDynamicModulesLoader> modules_loader,
  std::shared_ptr<avc::IAvcModuleLoadHandler> load_handler,
  const std::string &modules_path,
  const std::string &avcodec_module_name,
  const std::string &avformat_module_name,
  const std::string &avutil_module_name,
  const std::string &avdevice_module_name,
  const std::string &swscale_module_name,
  const std::string &swresample_module_name, 
  bool auto_load) {
  if (!modules_loader)
    modules_loader = CreateAvcDynamicModulesLoader();

  auto module_provider = std::make_shared<detail::AvcModuleProvider>(
        modules_loader,
        load_handler,
        modules_path,
        avcodec_module_name,
        avformat_module_name,
        avutil_module_name,
        avdevice_module_name,
        swscale_module_name,
        swresample_module_name);
  if (auto_load) {
    module_provider->Load();
  }
  return module_provider;
}

std::shared_ptr<IAvcModuleProvider> API_EXPORT CreateAvcModuleProvider3(
  const std::string& modules_path,
  bool auto_load,
  std::shared_ptr<avc::IAvcModuleLoadHandler> load_handler) {
  auto modules_loader = CreateAvcDynamicModulesLoader();
  auto module_provider = std::make_shared<detail::AvcModuleProvider>(modules_loader, load_handler, modules_path);
  if (auto_load) {
    module_provider->Load();
  }
  return module_provider;
}

std::shared_ptr<IAvcModuleProvider> API_EXPORT CreateAvcModuleProvider4(  
  const std::string& modules_path,
  const std::string& avcodec_module_name,
  const std::string& avformat_module_name,
  const std::string& avutil_module_name,
  const std::string& avdevice_module_name,
  const std::string& swscale_module_name,
  const std::string& swresample_module_name,
  bool auto_load,
  std::shared_ptr<avc::IAvcModuleLoadHandler> load_handler) {
  auto modules_loader = CreateAvcDynamicModulesLoader();
  auto module_provider = std::make_shared<detail::AvcModuleProvider>(
    modules_loader,
    load_handler,
    modules_path,
    avcodec_module_name,
    avformat_module_name,
    avutil_module_name,
    avdevice_module_name,
    swscale_module_name,
    swresample_module_name);
  if (auto_load) {
    module_provider->Load();
  }
  return module_provider;
}

namespace detail {

AvcModuleProvider::AvcModuleProvider(
  std::shared_ptr<cmf::IDynamicModulesLoader> modules_loader, std::shared_ptr<IAvcModuleLoadHandler> load_handler, const std::string &modules_path)
    : modules_loader_(modules_loader)
    , load_handler_(load_handler)
    , modules_path_(modules_path)
    , avcodec_handle_(nullptr)
    , avformat_handle_(nullptr)
    , avutil_handle_(nullptr)
    , avdevice_handle_(nullptr)
    , swscale_handle_(nullptr)
    , swresample_handle_(nullptr)
    , strict_modules_names_(false)
{
  avcodec_module_name_ = kDefaultAvCodecModuleName;
  avformat_module_name_ = kDefaultAvFormatModuleName;
  avutil_module_name_ = kDefaultAvUtilModuleName;
  avdevice_module_name_ = kDefaultAvDeviceModuleName;
  swscale_module_name_ = kDefaultSwScaleModuleName;
  swresample_module_name_ = kDefaultSwResampleModuleName;

  AvcDataProvidersGlobalInit();
}

AvcModuleProvider::AvcModuleProvider(
  std::shared_ptr<cmf::IDynamicModulesLoader> modules_loader,
  std::shared_ptr<IAvcModuleLoadHandler> load_handler,
  const std::string &modules_path,
  const std::string &avcodec_module_name,
  const std::string &avformat_module_name,
  const std::string &avutil_module_name,
  const std::string &avdevice_module_name,
  const std::string &swscale_module_name,
  const std::string &swresample_module_name,
  bool strict_modules_names)
    : modules_loader_(modules_loader)
    , load_handler_(load_handler)
    , strict_modules_names_(strict_modules_names)
    , modules_path_(modules_path)
    , avcodec_handle_(nullptr)
    , avformat_handle_(nullptr)
    , avutil_handle_(nullptr)
    , avdevice_handle_(nullptr)
    , swscale_handle_(nullptr)
    , swresample_handle_(nullptr)
    , avcodec_module_name_(avcodec_module_name)
    , avformat_module_name_(avformat_module_name)
    , avutil_module_name_(avutil_module_name)
    , avdevice_module_name_(avdevice_module_name)
    , swscale_module_name_(swscale_module_name)
    , swresample_module_name_(swresample_module_name) {
  if (avcodec_module_name_.size() == 0) 
    avcodec_module_name_ = kDefaultAvCodecModuleName;

  if (avformat_module_name_.size() == 0)
    avformat_module_name_ = kDefaultAvFormatModuleName;

  if (avutil_module_name_.size() == 0) 
    avutil_module_name_ = kDefaultAvUtilModuleName;

  if (avdevice_module_name_.size() == 0)
    avdevice_module_name_ = kDefaultAvDeviceModuleName;
  
  if (swscale_module_name_.size() == 0) 
    swscale_module_name_ = kDefaultSwScaleModuleName;

  if (swresample_module_name_.size() == 0)
    swresample_module_name_ = kDefaultSwResampleModuleName;

  AvcDataProvidersGlobalInit();
}

AvcModuleProvider::~AvcModuleProvider() { Unload(); }

bool AvcModuleProvider::IsAvCodecLoaded() const {
  return avcodec_handle_ != nullptr;
}

bool AvcModuleProvider::IsAvFormatLoaded() const {
  return avformat_handle_ != nullptr;
}

bool AvcModuleProvider::IsAvUtilLoaded() const {
  return avutil_handle_ != nullptr;
}

bool AvcModuleProvider::IsAvDeviceLoaded() const {
  return avdevice_handle_ != nullptr;
}

bool AvcModuleProvider::IsSwScaleLoaded() const {
  return swscale_handle_ != nullptr;
}

bool AvcModuleProvider::IsSwResampleLoaded() const {
  return swresample_handle_ != nullptr;
}

int AvcModuleProvider::CalculateVersionsScore(const AvcModuleVersion& required, const AvcModuleVersion& checked) {
  if (required.avcodec_version_ > 0 && checked.avcodec_version_ == 0)
    return -1;

  if (required.avdevice_version_ > 0 && checked.avdevice_version_ == 0)
    return -1;

  if (required.avformat_version_ > 0 && checked.avformat_version_ == 0)
    return -1;

  if (required.avutil_version_ > 0 && checked.avutil_version_ == 0)
    return -1;

  if (required.swresample_version_ > 0 && checked.swresample_version_ == 0)
    return -1;

  if (required.swscale_version_ > 0 && checked.swscale_version_ == 0)
    return -1;

  int avcodec_score = abs(static_cast<int>(required.avcodec_version_) - static_cast<int>(checked.avcodec_version_));
  if (required.avcodec_version_ == 0)
    avcodec_score = 0;

  int avdevice_score = abs(static_cast<int>(required.avdevice_version_) - static_cast<int>(checked.avdevice_version_));
  if (required.avdevice_version_ == 0)
    avdevice_score = 0;

  int avformat_score = abs(static_cast<int>(required.avformat_version_) - static_cast<int>(checked.avformat_version_));
  if (required.avformat_version_ == 0)
    avformat_score = 0;

  int avutil_score = abs(static_cast<int>(required.avutil_version_) - static_cast<int>(checked.avutil_version_));
  if (required.avutil_version_ == 0)
    avutil_score = 0;

  int swscale_score = abs(static_cast<int>(required.swscale_version_) - static_cast<int>(checked.swscale_version_));
  if (required.swscale_version_ == 0)
    swscale_score = 0;

  int swresample_score = abs(static_cast<int>(required.swresample_version_) - static_cast<int>(checked.swresample_version_));
  if (required.swresample_version_ == 0)
    swresample_score = 0;

  int score = avcodec_score + avdevice_score + avformat_score + avutil_score + swscale_score + swresample_score;
  return score;
}

void AvcModuleProvider::SetupDataWrapper() {
  AvcModuleVersion version;

  if (avcodec_handle_)
    version.avcodec_version_ = avcodec_version();

  if (avdevice_handle_)
    version.avdevice_version_ = avdevice_version();

  if (avformat_handle_)
    version.avformat_version_ = avformat_version();

  if (avutil_handle_)
    version.avutil_version_ = avutil_version();

  if (swresample_handle_)
    version.swresample_version_ = swresample_version();

  if (swscale_handle_)
    version.swscale_version_ = swscale_version();

  std::shared_ptr<IAvcModuleDataWrapperFactory> selected_factory = nullptr;

  // check full version equals
  for (const auto& factory : g_ffmpeg_data_wrappers) {
    AvcModuleVersion wrapper_ver = factory->GetVersion();
    if (wrapper_ver == version) {
      selected_factory = factory;
      break;
    }
  }

  if (!selected_factory) {
    std::vector<int> scores;
    scores.resize(g_ffmpeg_data_wrappers.size());

    for(size_t i=0; i<g_ffmpeg_data_wrappers.size(); i++) {
      auto factory = g_ffmpeg_data_wrappers[i];
      AvcModuleVersion wrapper_ver = factory->GetVersion();
      scores[i] = CalculateVersionsScore(version, wrapper_ver);
    }

    int min_score = 0x7FFFFFFE;
    size_t min_score_idx = 0;

    for (size_t i=0; i<scores.size(); i++) {
      if (scores[i] >= 0 && scores[i] < min_score) {
        min_score = scores[i];
        min_score_idx = i;
      }
    }

    selected_factory = g_ffmpeg_data_wrappers[min_score_idx];
  }

  if (!selected_factory) {
    printf("LIBAV data wrapper was not selected!");
  } else {
    AvcModuleVersion wrapver = selected_factory->GetVersion();
    std::shared_ptr<IAvcModuleDataWrapper> data_wrapper = selected_factory->Create(shared_from_this());
    data_wrapper_ = data_wrapper;
  }
}

bool AvcModuleProvider::LoadAvModule(const char* name, void** handle, const std::string& module_name, const std::string& noversion_module_name) {
  if (*handle != nullptr)
    return true;

  if (modules_path_.size()) {
    std::string path = modules_path_ + module_name;
    *handle = modules_loader_->LoadModule(path);

    if (*handle == nullptr && noversion_module_name.size()) {
      std::string path = modules_path_ + noversion_module_name;
      *handle = modules_loader_->LoadModule(path);
    }
  } 

  if (*handle == nullptr) {
    *handle = modules_loader_->LoadModule(module_name);

    if (*handle == nullptr && noversion_module_name.size()) {
      *handle = modules_loader_->LoadModule(noversion_module_name);
    }
  }

  if (*handle != nullptr) {
    return true;
  }

  printf("cannot load module %s: %s, path %s",
              name, module_name.c_str(), modules_path_.c_str());
  return false;
}

std::shared_ptr<IAvcModuleLoadHandler> AvcModuleProvider::GetLoadHandler() const {
  return load_handler_;
}

void AvcModuleProvider::SetLoadHandler(std::shared_ptr<IAvcModuleLoadHandler> load_handler) {
  load_handler_ = load_handler;
}

void AvcModuleProvider::Load() {
#ifdef AVC_LIBRARIES_STATIC_LINK
  LoadStatically();
#endif /*AVC_LIBRARIES_STATIC_LINK*/

  if (!avcodec_handle_) {
    if (LoadAvModule("AVCODEC", &avcodec_handle_, avcodec_module_name_, strict_modules_names_ ? std::string() : kNoVersionAvCodecModuleName)) {
      LoadAvCodecFunctions();
    } else {
      if (load_handler_) {
        load_handler_->OnModuleLoadError(shared_from_this(), "AVCODEC");
      }
    }
  }

  if (!avformat_handle_) {
    if (LoadAvModule("AVFORMAT", &avformat_handle_, avformat_module_name_, strict_modules_names_ ? std::string() : kNoVersionAvFormatModuleName)) {
      LoadAvFormatFunctions();
    } else {
      if (load_handler_) {
        load_handler_->OnModuleLoadError(shared_from_this(), "AVFORMAT");
      }
    }
  }

  if (!avutil_handle_) {
    if (LoadAvModule("AVUTIL", &avutil_handle_, avutil_module_name_, strict_modules_names_ ? std::string() : kNoVersionAvUtilModuleName)) {
      LoadAvUtilFunctions();
    } else {
      if (load_handler_) {
        load_handler_->OnModuleLoadError(shared_from_this(), "AVUTIL");
      }
    }
  }

  if (!avdevice_handle_) {
    if (LoadAvModule("AVDEVICE", &avdevice_handle_, avdevice_module_name_, strict_modules_names_ ? std::string() : kNoVersionAvDeviceModuleName)) {
      LoadAvDeviceFunctions();
    } else {
      if (load_handler_) {
        load_handler_->OnModuleLoadError(shared_from_this(), "AVDEVICE");
      }
    }
  }

  if (!swscale_handle_) {
    if (LoadAvModule("SWSCALE", &swscale_handle_, swscale_module_name_, strict_modules_names_ ? std::string() : kNoVersionSwScaleModuleName)) {
      LoadSwScaleFunctions();
    } else {
      if (load_handler_) {
        load_handler_->OnModuleLoadError(shared_from_this(), "SWSCALE");
      }
    }
  }

  if (!swresample_handle_) {
    if (LoadAvModule("SWRESAMPLE", &swresample_handle_, swresample_module_name_, strict_modules_names_ ? std::string() : kNoVersionSwResampleModuleName)) {
      LoadSwResampleFuctions();
    } else {
      if (load_handler_) {
        load_handler_->OnModuleLoadError(shared_from_this(), "SWRESAMPLE");
      }
    }
  }

  SetupDataWrapper();
}

void AvcModuleProvider::Unload() {
  if (load_handler_) {
    load_handler_->OnBeforeUnload(shared_from_this());
  }

#if !defined(AVC_LIBRARIES_STATIC_LINK) || AVC_LIBRARIES_STATIC_LINK==0
  if (avcodec_handle_) {
    modules_loader_->UnloadModule(avcodec_handle_);
    avcodec_handle_ = nullptr;
  }

  if (avformat_handle_) {
    modules_loader_->UnloadModule(avformat_handle_);
    avformat_handle_ = nullptr;
  }

  if (avutil_handle_) {
    modules_loader_->UnloadModule(avutil_handle_);
    avutil_handle_ = nullptr;
  }

  if (avdevice_handle_) {
    modules_loader_->UnloadModule(avdevice_handle_);
    avdevice_handle_ = nullptr;
  }

  if (swscale_handle_) {
    modules_loader_->UnloadModule(swscale_handle_);
    swscale_handle_ = nullptr;
  }

  if (swresample_handle_) {
    modules_loader_->UnloadModule(swresample_handle_);
    swresample_handle_ = nullptr;
  }
#endif //AVC_LIBRARIES_STATIC_LINK

  data_wrapper_ = nullptr;
}

void AvcModuleProvider::LoadAvCodecFunctions() {
#if !defined(AVC_LIBRARIES_STATIC_LINK) || AVC_LIBRARIES_STATIC_LINK==0
  DynamicLoaderHelper avcodec_loader(modules_loader_);
  avcodec_loader
    .SetModuleHandle(avcodec_handle_)
    .LoadProc("avcodec_version", avcodec_version_)
    .LoadProc("avcodec_encode_video2", avcodec_encode_video2_)
    .LoadProc("av_get_codec_tag_string", av_get_codec_tag_string_)
    .LoadProc("avcodec_parameters_copy", avcodec_parameters_copy_)
    .LoadProc("avcodec_parameters_from_context", avcodec_parameters_from_context_)
    .LoadProc("avcodec_parameters_free", avcodec_parameters_free_)
    .LoadProc("avcodec_parameters_alloc", avcodec_parameters_alloc_)
    .LoadProc("avcodec_parameters_to_context", avcodec_parameters_to_context_)
    .LoadProc("av_register_hwaccel", av_register_hwaccel_)
    .LoadProc("av_hwaccel_next", av_hwaccel_next_)
    .LoadProc("av_lockmgr_register", av_lockmgr_register_)
    .LoadProc("av_packet_alloc", av_packet_alloc_)
    .LoadProc("av_packet_clone", av_packet_clone_)
    .LoadProc("av_packet_free", av_packet_free_)
    .LoadProc("av_init_packet", av_init_packet_)
    .LoadProc("av_new_packet", av_new_packet_)
    .LoadProc("av_packet_ref", av_packet_ref_)
    .LoadProc("av_packet_unref", av_packet_unref_)
    .LoadProc("avcodec_alloc_context3", avcodec_alloc_context3_)
    .LoadProc("avcodec_free_context", avcodec_free_context_)
    .LoadProc("avcodec_find_decoder", avcodec_find_decoder_)
    .LoadProc("avcodec_find_decoder_by_name", avcodec_find_decoder_by_name_)
    .LoadProc("avcodec_find_encoder", avcodec_find_encoder_)
    .LoadProc("avcodec_find_encoder_by_name", avcodec_find_encoder_by_name_)
    .LoadProc("avcodec_flush_buffers", avcodec_flush_buffers_)
    .LoadProc("avcodec_get_name", avcodec_get_name_)
    .LoadProc("av_codec_is_encoder", av_codec_is_encoder_)
    .LoadProc("av_codec_is_decoder", av_codec_is_decoder_)
    .LoadProc("avcodec_open2", avcodec_open2_)
    .LoadProc("avcodec_receive_frame", avcodec_receive_frame_)
    .LoadProc("avcodec_send_frame", avcodec_send_frame_)
    .LoadProc("avcodec_receive_packet", avcodec_receive_packet_)
    .LoadProc("avcodec_send_packet", avcodec_send_packet_)
    .LoadProc("avcodec_register_all", avcodec_register_all_)
    .LoadProc("av_codec_iterate", av_codec_iterate_)
    .LoadProc("avcodec_get_hw_config", avcodec_get_hw_config_)
    ;

  if (avcodec_loader.HasErrors()) {
    printf("avcodec functions were not loaded: %s", avcodec_loader.ProblemFunctions().str().c_str());
  }
#endif //AV_LIBRARIES_STATIC_LINK
}

void AvcModuleProvider::LoadAvFormatFunctions() {
#if !defined(AVC_LIBRARIES_STATIC_LINK) || AVC_LIBRARIES_STATIC_LINK==0
  DynamicLoaderHelper avformat_loader(modules_loader_);
  avformat_loader
    .SetModuleHandle(avformat_handle_)
    .LoadProc("avformat_version", avformat_version_)
    .LoadProc("av_dump_format", av_dump_format_)
    .LoadProc("av_find_input_format", av_find_input_format_)
    .LoadProc("av_guess_format", av_guess_format_)
    .LoadProc("av_read_frame", av_read_frame_)
    .LoadProc("av_read_play", av_read_play_)
    .LoadProc("av_read_pause", av_read_pause_)
    .LoadProc("av_register_all", av_register_all_)
    .LoadProc("avformat_flush", avformat_flush_)
    .LoadProc("av_seek_frame", av_seek_frame_)
    .LoadProc("av_write_frame", av_write_frame_)
    .LoadProc("av_interleaved_write_frame", av_interleaved_write_frame_)
    .LoadProc("av_write_trailer", av_write_trailer_)
    .LoadProc("avformat_alloc_context", avformat_alloc_context_)
    .LoadProc("avformat_alloc_output_context2", avformat_alloc_output_context2_)
    .LoadProc("avformat_free_context", avformat_free_context_)
    .LoadProc("avformat_close_input", avformat_close_input_)
    .LoadProc("avformat_find_stream_info", avformat_find_stream_info_)
    .LoadProc("avformat_network_init", avformat_network_init_)
    .LoadProc("avformat_network_deinit", avformat_network_deinit_)
    .LoadProc("avformat_new_stream", avformat_new_stream_)
    .LoadProc("av_stream_add_side_data", av_stream_add_side_data_)
    .LoadProc("av_stream_new_side_data", av_stream_new_side_data_)
    .LoadProc("av_stream_get_side_data", av_stream_get_side_data_)
    .LoadProc("avformat_open_input", avformat_open_input_)
    .LoadProc("avformat_write_header", avformat_write_header_)
    .LoadProc("avio_alloc_context", avio_alloc_context_)
    .LoadProc("avio_context_free", avio_context_free_)
    .LoadProc("avio_close", avio_close_)
    .LoadProc("avio_closep", avio_closep_)
    .LoadProc("avio_flush", avio_flush_)
    .LoadProc("avio_open2", avio_open2_)
    ;

  if (avformat_loader.HasErrors()) {
    printf("avformat functions were not loaded: %s", avformat_loader.ProblemFunctions().str().c_str());
  }
#endif //AV_LIBRARIES_STATIC_LINK
}

void AvcModuleProvider::LoadAvUtilFunctions() {
#if !defined(AVC_LIBRARIES_STATIC_LINK) || AVC_LIBRARIES_STATIC_LINK==0
  DynamicLoaderHelper avutil_loader(modules_loader_);
  avutil_loader
    .SetModuleHandle(avutil_handle_)
    .LoadProc("avutil_version", avutil_version_)
    .LoadProc("av_samples_get_buffer_size", av_samples_get_buffer_size_)
    .LoadProc("av_rescale_rnd", av_rescale_rnd_)
    .LoadProc("av_samples_alloc", av_samples_alloc_)
    .LoadProc("av_samples_alloc_array_and_samples", av_samples_alloc_array_and_samples_)
    .LoadProc("av_opt_set_int", av_opt_set_int_)
    .LoadProc("av_opt_set_sample_fmt", av_opt_set_sample_fmt_)
    .LoadProc("av_hwdevice_ctx_alloc", av_hwdevice_ctx_alloc_)
    .LoadProc("av_hwdevice_ctx_init", av_hwdevice_ctx_init_)
    .LoadProc("av_hwframe_ctx_alloc", av_hwframe_ctx_alloc_)
    .LoadProc("av_hwdevice_find_type_by_name", av_hwdevice_find_type_by_name_)
    .LoadProc("av_hwdevice_get_type_name", av_hwdevice_get_type_name_)
    .LoadProc("av_hwdevice_iterate_types", av_hwdevice_iterate_types_)
    .LoadProc("av_hwdevice_ctx_create", av_hwdevice_ctx_create_)
    .LoadProc("av_hwframe_ctx_init", av_hwframe_ctx_init_)
    .LoadProc("av_hwframe_get_buffer", av_hwframe_get_buffer_)
    .LoadProc("av_hwframe_transfer_data", av_hwframe_transfer_data_)
    .LoadProc("av_hwframe_transfer_get_formats", av_hwframe_transfer_get_formats_)
    .LoadProc("av_hwdevice_hwconfig_alloc", av_hwdevice_hwconfig_alloc_)
    .LoadProc("av_hwdevice_get_hwframe_constraints", av_hwdevice_get_hwframe_constraints_)
    .LoadProc("av_hwframe_constraints_free", av_hwframe_constraints_free_)
    .LoadProc("av_hwframe_map", av_hwframe_map_)
    .LoadProc("av_dict_set", av_dict_set_)
    .LoadProc("av_dict_set_int", av_dict_set_int_)
    .LoadProc("av_dict_get", av_dict_get_)
    .LoadProc("av_dict_count", av_dict_count_)
    .LoadProc("av_dict_free", av_dict_free_)
    .LoadProc("av_frame_alloc", av_frame_alloc_)
    .LoadProc("av_frame_free", av_frame_free_)
    .LoadProc("av_frame_get_buffer", av_frame_get_buffer_)
    .LoadProc("av_frame_get_channels", av_frame_get_channels_)
    .LoadProc("av_frame_set_channels", av_frame_set_channels_)
    .LoadProc("av_frame_get_pkt_duration", av_frame_get_pkt_duration_)
    .LoadProc("av_frame_set_pkt_duration", av_frame_set_pkt_duration_)
    .LoadProc("av_frame_get_pkt_pos", av_frame_get_pkt_pos_)
    .LoadProc("av_frame_set_pkt_pos", av_frame_set_pkt_pos_)
    .LoadProc("av_frame_get_sample_rate", av_frame_get_sample_rate_)
    .LoadProc("av_frame_set_sample_rate", av_frame_set_sample_rate_)
    .LoadProc("av_free", av_free_)
    .LoadProc("av_freep", av_freep_)
    .LoadProc("av_get_bytes_per_sample", av_get_bytes_per_sample_)
    .LoadProc("av_image_copy_to_buffer", av_image_copy_to_buffer_)
    .LoadProc("av_image_fill_arrays", av_image_fill_arrays_)
    .LoadProc("av_image_get_buffer_size", av_image_get_buffer_size_)
    .LoadProc("av_log_default_callback", av_log_default_callback_)
    .LoadProc("av_log_set_callback", av_log_set_callback_)
    .LoadProc("av_log_set_level", av_log_set_level_)
    .LoadProc("av_malloc", av_malloc_)
    .LoadProc("av_strdup", av_strdup_)
    .LoadProc("av_pix_fmt_count_planes", av_pix_fmt_count_planes_)
    .LoadProc("av_pix_fmt_desc_get", av_pix_fmt_desc_get_)
    .LoadProc("av_sample_fmt_is_planar", av_sample_fmt_is_planar_)
    .LoadProc("av_samples_set_silence", av_samples_set_silence_)
    .LoadProc("av_strerror", av_strerror_)
    .LoadProc("av_buffer_create", av_buffer_create_)
    .LoadProc("av_buffer_is_writable", av_buffer_is_writable_)
    .LoadProc("av_buffer_get_opaque", av_buffer_get_opaque_)
    .LoadProc("av_buffer_get_ref_count", av_buffer_get_ref_count_)
    .LoadProc("av_buffer_make_writable", av_buffer_make_writable_)
    .LoadProc("av_buffer_realloc", av_buffer_realloc_)
    .LoadProc("av_buffer_unref", av_buffer_unref_)
    .LoadProc("av_buffer_ref", av_buffer_ref_)
    .LoadProc("av_get_channel_layout", av_get_channel_layout_)
    .LoadProc("av_get_channel_layout_nb_channels", av_get_channel_layout_nb_channels_)
    .LoadProc("av_get_default_channel_layout", av_get_default_channel_layout_)
    .LoadProc("av_get_channel_layout_channel_index", av_get_channel_layout_channel_index_)
    .LoadProc("av_channel_layout_extract_channel", av_channel_layout_extract_channel_)
    .LoadProc("av_get_channel_name", av_get_channel_name_)
    .LoadProc("av_get_channel_description", av_get_channel_description_)
    .LoadProc("av_get_standard_channel_layout", av_get_standard_channel_layout_)
    ;

  if (avutil_loader.HasErrors()) {
    printf("avutil functions were not loaded: %s", avutil_loader.ProblemFunctions().str().c_str());
  }
#endif //AV_LIBRARIES_STATIC_LINK
}

void AvcModuleProvider::LoadSwScaleFunctions() {
#if !defined(AVC_LIBRARIES_STATIC_LINK) || AVC_LIBRARIES_STATIC_LINK==0
  DynamicLoaderHelper swscale_loader(modules_loader_);
  swscale_loader
    .SetModuleHandle(swscale_handle_)
    .LoadProc("swscale_version", swscale_version_)
    .LoadProc("sws_freeContext", sws_freeContext_)
    .LoadProc("sws_getContext", sws_getContext_)
    .LoadProc("sws_scale", sws_scale_)
    ;

  if (swscale_loader.HasErrors()) {
    printf("swscale functions were not loaded: %s", swscale_loader.ProblemFunctions().str().c_str());
  }
#endif //AV_LIBRARIES_STATIC_LINK
}

void AvcModuleProvider::LoadSwResampleFuctions() {
#if !defined(AVC_LIBRARIES_STATIC_LINK) || AVC_LIBRARIES_STATIC_LINK==0
  DynamicLoaderHelper swresample_loader(modules_loader_);
  swresample_loader
    .SetModuleHandle(swresample_handle_)
    .LoadProc("swresample_version", swresample_version_)
    .LoadProc("swr_alloc", swr_alloc_)
    .LoadProc("swr_init", swr_init_)
    .LoadProc("swr_is_initialized", swr_is_initialized_)
    .LoadProc("swr_free", swr_free_)
    .LoadProc("swr_close", swr_close_)
    .LoadProc("swr_convert", swr_convert_)
    .LoadProc("swr_get_delay", swr_get_delay_)
    ;

  if (swresample_loader.HasErrors()) {
    printf("swresample functions were not loaded: %s", swresample_loader.ProblemFunctions().str().c_str());
  }
#endif //AV_LIBRARIES_STATIC_LINK
}

void AvcModuleProvider::LoadAvDeviceFunctions() {
#if !defined(AVC_LIBRARIES_STATIC_LINK) || AVC_LIBRARIES_STATIC_LINK==0
  DynamicLoaderHelper avdevice_loader(modules_loader_);
  avdevice_loader
    .SetModuleHandle(avdevice_handle_)
    .LoadProc("avdevice_version", avdevice_version_)
    .LoadProc("avdevice_register_all", avdevice_register_all_)
    ;

  if (avdevice_loader.HasErrors()) {
    printf("avdevice functions were not loaded: %s", avdevice_loader.ProblemFunctions().str().c_str());
  }
#endif //AV_LIBRARIES_STATIC_LINK
}

// avcodec
unsigned AvcModuleProvider::avcodec_version() {
  if (!avcodec_handle_) Load();
  return avcodec_version_();
}

int AvcModuleProvider::avcodec_encode_video2(AVCodecContext *avctx, AVPacket *avpkt,
                                             const AVFrame *frame, int *got_packet_ptr) {
  if (!avcodec_handle_) Load();
  return avcodec_encode_video2_(avctx, avpkt, frame, got_packet_ptr);
}

size_t AvcModuleProvider::av_get_codec_tag_string(char *buf, size_t buf_size,
                                                  unsigned int codec_tag) {
  if (!avcodec_handle_) Load();
  return av_get_codec_tag_string_(buf, buf_size, codec_tag);
}

int AvcModuleProvider::avcodec_parameters_copy(AVCodecParameters *dst,
                                               const AVCodecParameters *src) {
  if (!avcodec_handle_) Load();
  return avcodec_parameters_copy_(dst, src);
}

int AvcModuleProvider::avcodec_parameters_from_context(AVCodecParameters *par,
                                                       const AVCodecContext *codec) {
  if (!avcodec_handle_) Load();
  return avcodec_parameters_from_context_(par, codec);
}

void AvcModuleProvider::avcodec_parameters_free(AVCodecParameters **par) {
  if (!avcodec_handle_) Load();
  avcodec_parameters_free_(par);
}

AVCodecParameters *AvcModuleProvider::avcodec_parameters_alloc() {
  if (!avcodec_handle_) Load();
  return avcodec_parameters_alloc_();
}

int AvcModuleProvider::avcodec_parameters_to_context(AVCodecContext *codec,
                                                     const AVCodecParameters *par) {
  if (!avcodec_handle_) Load();
  return avcodec_parameters_to_context_(codec, par);
}

void AvcModuleProvider::av_register_hwaccel(AVHWAccel *hwaccel) {
  if (!avcodec_handle_) Load();
  if (av_register_hwaccel_)
    av_register_hwaccel_(hwaccel);
}

AVHWAccel *AvcModuleProvider::av_hwaccel_next(const AVHWAccel *hwaccel) {
  if (!avcodec_handle_) Load();
  return av_hwaccel_next_(hwaccel);
}

int AvcModuleProvider::av_lockmgr_register(int (*cb)(void **mutex,
                                                     int /*enum AVLockOp*/ op)) {
  if (!avcodec_handle_) Load();
  if (!av_lockmgr_register_)
    return 0;

  return av_lockmgr_register_(
    reinterpret_cast<int (*)(void **, /*enum AVLockOp*/ int)>(cb));
}

AVPacket *AvcModuleProvider::av_packet_alloc(void) {
  if (!avcodec_handle_) Load();
  return av_packet_alloc_();
}

AVPacket *AvcModuleProvider::av_packet_clone(AVPacket *src) {
  if (!avcodec_handle_) Load();
  return av_packet_clone_(src);
}

void AvcModuleProvider::av_packet_free(AVPacket **pkt) {
  if (!avcodec_handle_) Load();
  return av_packet_free_(pkt);
}

void AvcModuleProvider::av_init_packet(AVPacket *pkt) {
  if (!avcodec_handle_) Load();
  av_init_packet_(pkt);
}

int AvcModuleProvider::av_new_packet(AVPacket *pkt, int size) {
  if (!avcodec_handle_) Load();
  return av_new_packet_(pkt, size);
}

void AvcModuleProvider::av_packet_ref(AVPacket *dst, const AVPacket* src) {
  if (!avcodec_handle_) Load();
  av_packet_ref_(dst, src);
}

void AvcModuleProvider::av_packet_unref(AVPacket *pkt) {
  if (!avcodec_handle_) Load();
  av_packet_unref_(pkt);
}

AVCodecContext *AvcModuleProvider::avcodec_alloc_context3(const AVCodec *codec) {
  if (!avcodec_handle_) Load();
  return avcodec_alloc_context3_(codec);
}

void AvcModuleProvider::avcodec_free_context(AVCodecContext **avctx) {
  if (!avcodec_handle_) Load();
  avcodec_free_context_(avctx);
}

AVCodec *AvcModuleProvider::avcodec_find_decoder(int id) {
  if (!avcodec_handle_) Load();
  return avcodec_find_decoder_(id);
}
AVCodec *AvcModuleProvider::avcodec_find_decoder_by_name(const char *name) {
  if (!avcodec_handle_) Load();
  return avcodec_find_decoder_by_name_(name);
}

AVCodec *AvcModuleProvider::avcodec_find_encoder(int /*enum AVCodecID*/ id) {
  if (!avcodec_handle_) Load();
  return avcodec_find_encoder_(id);
}

AVCodec *AvcModuleProvider::avcodec_find_encoder_by_name(const char *name) {
  if (!avcodec_handle_) Load();
  return avcodec_find_encoder_by_name_(name);
}
void AvcModuleProvider::avcodec_flush_buffers(AVCodecContext *avctx) {
  if (!avcodec_handle_) Load();
  avcodec_flush_buffers_(avctx);
}

const char *AvcModuleProvider::avcodec_get_name(int /*enum AVCodecID*/ id) {
  if (!avcodec_handle_) Load();
  return avcodec_get_name_(id);
}
int AvcModuleProvider::av_codec_is_encoder(const AVCodec *codec) {
  if (!avcodec_handle_) Load();
  return av_codec_is_encoder_(codec);
}
int AvcModuleProvider::av_codec_is_decoder(const AVCodec *codec) {
  if (!avcodec_handle_) Load();
  return av_codec_is_decoder_(codec);
}
int AvcModuleProvider::avcodec_open2(AVCodecContext *avctx, const AVCodec *codec,
                                     AVDictionary **options) {
  if (!avcodec_handle_) Load();
  return avcodec_open2_(avctx, codec, options);
}
int AvcModuleProvider::avcodec_receive_frame(AVCodecContext *avctx, AVFrame *frame) {
  if (!avcodec_handle_) Load();
  return avcodec_receive_frame_(avctx, frame);
}
int AvcModuleProvider::avcodec_send_frame(AVCodecContext *avctx, const AVFrame *frame) {
  if (!avcodec_handle_) Load();
  return avcodec_send_frame_(avctx, frame);
}
int AvcModuleProvider::avcodec_receive_packet(AVCodecContext *avctx, AVPacket *avpkt) {
  if (!avcodec_handle_) Load();
  return avcodec_receive_packet_(avctx, avpkt);
}

int AvcModuleProvider::avcodec_send_packet(AVCodecContext *avctx, const AVPacket *avpkt) {
  if (!avcodec_handle_) Load();
  return avcodec_send_packet_(avctx, avpkt);
}

void AvcModuleProvider::avcodec_register_all() {
  if (!avcodec_handle_) Load();
  if (avcodec_register_all_)
    avcodec_register_all_();
}

const AVCodec *AvcModuleProvider::av_codec_iterate(void **opaque) {
  if (!avcodec_handle_) Load();
  return av_codec_iterate_(opaque);
}

const AVCodecHWConfig *AvcModuleProvider::avcodec_get_hw_config(const AVCodec *codec,
                                                                int index) {
  if (!avcodec_handle_) Load();
  return avcodec_get_hw_config_(codec, index);
}
//// avformat

unsigned AvcModuleProvider::avformat_version() {
  if (!avformat_handle_) Load();
  return avformat_version_();
}

void AvcModuleProvider::av_dump_format(AVFormatContext *ic, int index, const char *url,
                                       int is_output) {
  if (!avformat_handle_) Load();
  return av_dump_format_(ic, index, url, is_output);
}

AVInputFormat *AvcModuleProvider::av_find_input_format(const char *short_name) {
  if (!avformat_handle_) Load();
  return av_find_input_format_(short_name);
}

AVOutputFormat *AvcModuleProvider::av_guess_format(const char *short_name,
                                                   const char *filename,
                                                   const char *mime_type) {
  if (!avformat_handle_) Load();
  return av_guess_format_(short_name, filename, mime_type);
}

int AvcModuleProvider::av_read_frame(AVFormatContext *s, AVPacket *pkt) {
  if (!avformat_handle_) Load();
  return av_read_frame_(s, pkt);
}

int AvcModuleProvider::av_read_play(AVFormatContext *s) {
  if (!avformat_handle_) Load();
  return av_read_play_(s);
}

int AvcModuleProvider::av_read_pause(AVFormatContext *s) {
  if (!avformat_handle_) Load();
  return av_read_pause_(s);
}

void AvcModuleProvider::av_register_all(void) {
  if (!avformat_handle_) Load();
  if (av_register_all_)
    av_register_all_();
}

int AvcModuleProvider::avformat_flush(AVFormatContext *s) {
  if (!avformat_handle_) Load();
  return avformat_flush_(s);
}

int AvcModuleProvider::av_seek_frame(AVFormatContext *s, int stream_index,
                                     int64_t timestamp, int flags) {
  if (!avformat_handle_) Load();
  return av_seek_frame_(s, stream_index, timestamp, flags);
}

int AvcModuleProvider::av_write_frame(AVFormatContext *s, AVPacket *pkt) {
  if (!avformat_handle_) Load();
  return av_write_frame_(s, pkt);
}

int AvcModuleProvider::av_interleaved_write_frame(AVFormatContext *s, AVPacket *pkt) {
  if (!avformat_handle_) Load();
  return av_interleaved_write_frame_(s, pkt);
}

int AvcModuleProvider::av_write_trailer(AVFormatContext *s) {
  if (!avformat_handle_) Load();
  return av_write_trailer_(s);
}

AVFormatContext *AvcModuleProvider::avformat_alloc_context(void) {
  if (!avformat_handle_) Load();
  return avformat_alloc_context_();
}

int AvcModuleProvider::avformat_alloc_output_context2(AVFormatContext **ctx,
                                                      AVOutputFormat *oformat,
                                                      const char *format_name,
                                                      const char *filename) {
  if (!avformat_handle_) Load();
  return avformat_alloc_output_context2_(ctx, oformat, format_name, filename);
}

void AvcModuleProvider::avformat_free_context(AVFormatContext *s) {
  if (!avformat_handle_) Load();
  avformat_free_context_(s);
}

void AvcModuleProvider::avformat_close_input(AVFormatContext **s) {
  if (!avformat_handle_) Load();
  avformat_close_input_(s);
}

int AvcModuleProvider::avformat_find_stream_info(AVFormatContext *ic,
                                                 AVDictionary **options) {
  if (!avformat_handle_) Load();
  return avformat_find_stream_info_(ic, options);
}

int AvcModuleProvider::avformat_network_init(void) {
  if (!avformat_handle_) Load();
  return avformat_network_init_();
}

int AvcModuleProvider::avformat_network_deinit(void) {
  if (!avformat_handle_) Load();
  return avformat_network_deinit_();
}

AVStream *AvcModuleProvider::avformat_new_stream(AVFormatContext *s, const AVCodec *c) {
  if (!avformat_handle_) Load();
  return avformat_new_stream_(s, c);
}

int AvcModuleProvider::av_stream_add_side_data(AVStream *st, int type,
                                               uint8_t *data, size_t size) {
  if (!avformat_handle_) Load();
  return av_stream_add_side_data_(st, type, data, size);
}

uint8_t* AvcModuleProvider::av_stream_new_side_data(AVStream *stream,
                                                    int type, int size) {
  if (!avformat_handle_) Load();
  return av_stream_new_side_data_(stream, type, size);
}

uint8_t* AvcModuleProvider::av_stream_get_side_data(const AVStream *stream,
                                                    int type, int *size) {
  if (!avformat_handle_) Load();
  return av_stream_get_side_data_(stream, type, size);
}

int AvcModuleProvider::avformat_open_input(AVFormatContext **ps, const char *url,
                                           AVInputFormat *fmt, AVDictionary **options) {
  if (!avformat_handle_) Load();
  return avformat_open_input_(ps, url, fmt, options);
}

int AvcModuleProvider::avformat_write_header(AVFormatContext *s, AVDictionary **options) {
  if (!avformat_handle_) Load();
  return avformat_write_header_(s, options);
}

AVIOContext *AvcModuleProvider::avio_alloc_context(
  unsigned char *buffer, int buffer_size, int write_flag, void *opaque,
  int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),
  int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),
  int64_t (*seek)(void *opaque, int64_t offset, int whence)) {
  if (!avformat_handle_) Load();
  return avio_alloc_context_(buffer, buffer_size, write_flag, opaque, read_packet,
                             write_packet, seek);
}

void AvcModuleProvider::avio_context_free(AVIOContext** s) {
  if (!avformat_handle_) Load();
  return avio_context_free_(s);
}

int AvcModuleProvider::avio_close(AVIOContext *s) {
  if (!avformat_handle_) Load();
  return avio_close_(s);
}

int AvcModuleProvider::avio_closep(AVIOContext **s) {
  if (!avformat_handle_) Load();
  return avio_closep_(s);
}

void AvcModuleProvider::avio_flush(AVIOContext *s) {
  if (!avformat_handle_) Load();
  avio_flush_(s);
}

int AvcModuleProvider::avio_open2(AVIOContext **s, const char *url, int flags,
                                  const AVIOInterruptCB *int_cb, AVDictionary **options) {
  if (!avformat_handle_) Load();
  return avio_open2_(s, url, flags, int_cb, options);
}

// avutils

unsigned AvcModuleProvider::avutil_version() {
  if (!avutil_handle_) Load();
  return avutil_version_();
}

char *AvcModuleProvider::av_strdup(const char *s) { return av_strdup_(s); }

int AvcModuleProvider::av_samples_get_buffer_size(int *linesize, int nb_channels,
                                                  int nb_samples,
                                                  int /*enum AVSampleFormat*/ sample_fmt,
                                                  int align) {
  if (!avutil_handle_) Load();
  return av_samples_get_buffer_size_(linesize, nb_channels, nb_samples, sample_fmt,
                                     align);
}

int64_t AvcModuleProvider::av_rescale_rnd(int64_t a, int64_t b, int64_t c,
                                          int /*enum AVRounding*/ rnd) {
  if (!avutil_handle_) Load();
  return av_rescale_rnd_(a, b, c, rnd);
}

int AvcModuleProvider::av_samples_alloc(uint8_t **audio_data, int *linesize,
                                        int nb_channels, int nb_samples,
                                        int /*enum AVSampleFormat*/ sample_fmt,
                                        int /*(0 = default, 1 = no alignment)*/ align) {
  if (!avutil_handle_) Load();
  return av_samples_alloc_(audio_data, linesize, nb_channels, nb_samples, sample_fmt,
                           align);
}

int AvcModuleProvider::av_samples_alloc_array_and_samples(
  uint8_t ***audio_data, int *linesize, int nb_channels, int nb_samples,
  int /*enum AVSampleFormat*/ sample_fmt, int /*(0 = default, 1 = no alignment)*/ align) {
  if (!avutil_handle_) Load();
  return av_samples_alloc_array_and_samples_(audio_data, linesize, nb_channels,
                                             nb_samples, sample_fmt, align);
}

int AvcModuleProvider::av_opt_set_int(void *obj, const char *name, int64_t val,
                                      int search_flags) {
  if (!avutil_handle_) Load();
  return av_opt_set_int_(obj, name, val, search_flags);
}

int AvcModuleProvider::av_opt_set_sample_fmt(void *obj, const char *name,
                                             int /*enum AVSampleFormat*/ fmt,
                                             int search_flags) {
  if (!avutil_handle_) Load();
  return av_opt_set_sample_fmt_(obj, name, fmt, search_flags);
}

AVBufferRef *AvcModuleProvider::av_hwdevice_ctx_alloc(int /*enum AVHWDeviceType*/ type) {
  if (!avutil_handle_) Load();
  return av_hwdevice_ctx_alloc_(type);
}

int AvcModuleProvider::av_hwdevice_ctx_init(AVBufferRef *ref) {
  if (!avutil_handle_) Load();
  return av_hwdevice_ctx_init_(ref);
}

AVBufferRef *AvcModuleProvider::av_hwframe_ctx_alloc(AVBufferRef *device_ctx) {
  if (!avutil_handle_) Load();
  return av_hwframe_ctx_alloc_(device_ctx);
}

int AvcModuleProvider::av_hwdevice_find_type_by_name(const char *name) {
  if (!avutil_handle_) Load();
  return av_hwdevice_find_type_by_name_(name);
}

const char *AvcModuleProvider::av_hwdevice_get_type_name(int type) {
  if (!avutil_handle_) Load();
  return av_hwdevice_get_type_name_(type);
}

int /*enum AVHWDeviceType*/ AvcModuleProvider::av_hwdevice_iterate_types(
  int /* enum AVHWDeviceType*/ prev) {
  if (!avutil_handle_) Load();
  return av_hwdevice_iterate_types_(prev);
}

int AvcModuleProvider::av_hwdevice_ctx_create(AVBufferRef **device_ctx, int type,
                                              const char *device, AVDictionary *opts,
                                              int flags) {
  if (!avutil_handle_) Load();
  return av_hwdevice_ctx_create_(device_ctx, type, device, opts, flags);
}

int AvcModuleProvider::av_hwframe_ctx_init(AVBufferRef *ref) {
  if (!avutil_handle_) Load();
  return av_hwframe_ctx_init_(ref);
}

int AvcModuleProvider::av_hwframe_get_buffer(AVBufferRef *hwframe_ctx, AVFrame *frame,
                                             int flags) {
  if (!avutil_handle_) Load();
  return av_hwframe_get_buffer_(hwframe_ctx, frame, flags);
}

int AvcModuleProvider::av_hwframe_transfer_data(AVFrame *dst, const AVFrame *src,
                                                int flags) {
  if (!avutil_handle_) Load();
  return av_hwframe_transfer_data_(dst, src, flags);
}

int AvcModuleProvider::av_hwframe_transfer_get_formats(AVBufferRef *hwframe_ctx, int dir,
                                                       int **formats, int flags) {
  if (!avutil_handle_) Load();
  return av_hwframe_transfer_get_formats_(hwframe_ctx, dir, formats, flags);
}

void *AvcModuleProvider::av_hwdevice_hwconfig_alloc(AVBufferRef *device_ctx) {
  if (!avutil_handle_) Load();
  return av_hwdevice_hwconfig_alloc_(device_ctx);
}

AVHWFramesConstraints *AvcModuleProvider::av_hwdevice_get_hwframe_constraints(
  AVBufferRef *ref, const void *hwconfig) {
  if (!avutil_handle_) Load();
  return av_hwdevice_get_hwframe_constraints_(ref, hwconfig);
}

void AvcModuleProvider::av_hwframe_constraints_free(AVHWFramesConstraints **constraints) {
  if (!avutil_handle_) Load();
  av_hwframe_constraints_free_(constraints);
}

int AvcModuleProvider::av_hwframe_map(AVFrame *dst, const AVFrame *src, int flags) {
  if (!avutil_handle_) Load();
  return av_hwframe_map_(dst, src, flags);
}

int AvcModuleProvider::av_dict_set(AVDictionary **pm, const char *key, const char *value,
                                   int flags) {
  if (!avutil_handle_) Load();
  return av_dict_set_(pm, key, value, flags);
}

int AvcModuleProvider::av_dict_set_int(AVDictionary **pm, const char *key, int64_t value,
                                       int flags) {
  if (!avutil_handle_) Load();
  return av_dict_set_int_(pm, key, value, flags);
}

AVDictionaryEntry *AvcModuleProvider::av_dict_get(const AVDictionary *m, const char *key,
                                                  const AVDictionaryEntry *prev,
                                                  int flags) {
  if (!avutil_handle_) Load();
  return av_dict_get_(m, key, prev, flags);
}

int AvcModuleProvider::av_dict_count(const AVDictionary *m) {
  if (!avutil_handle_) Load();
  return av_dict_count_(m);
}

void AvcModuleProvider::av_dict_free(AVDictionary **m) {
  if (!avutil_handle_) Load();
  av_dict_free_(m);
}

AVFrame *AvcModuleProvider::av_frame_alloc(void) {
  if (!avutil_handle_) Load();
  return av_frame_alloc_();
}

void AvcModuleProvider::av_frame_free(AVFrame **frame) {
  if (!avutil_handle_) Load();
  av_frame_free_(frame);
}

int AvcModuleProvider::av_frame_get_buffer(AVFrame *frame, int align) {
  if (!avutil_handle_) Load();
  return av_frame_get_buffer_(frame, align);
}

int AvcModuleProvider::av_frame_get_channels(const AVFrame *frame) {
  if (!avutil_handle_) Load();

  if (av_frame_get_channels_)
    return av_frame_get_channels_(frame);
  else {
#if LIBAVUTIL_VERSION_MAJOR>57
    return frame->ch_layout.nb_channels;
#else //LIBAVUTIL_VERSION_MAJOR
    return d()->AVFrameGetChannels(frame);
#endif //LIBAVUTIL_VERSION_MAJOR
  }
}

void AvcModuleProvider::av_frame_set_channels(AVFrame *frame, int val) {
  if (!avutil_handle_) Load();

  if (av_frame_set_channels_) {
    av_frame_set_channels_(frame, val);
  } else {
#if LIBAVUTIL_VERSION_MAJOR>57
    frame->ch_layout.nb_channels = val;
    frame->ch_layout.order = AV_CHANNEL_ORDER_UNSPEC;
#else //LIBAVUTIL_VERSION_MAJOR
    d()->AVFrameSetChannels(frame,val);
#endif //LIBAVUTIL_VERSION_MAJOR
  }
}

int64_t AvcModuleProvider::av_frame_get_pkt_duration(const AVFrame *frame) {
  if (!avutil_handle_) Load();

  if (av_frame_get_pkt_duration_) {
    return av_frame_get_pkt_duration_(frame);
  } else {
    return d()->AVFrameGetPktDuration(frame);
  }
}

void AvcModuleProvider::av_frame_set_pkt_duration(AVFrame *frame, int64_t val) {
  if (!avutil_handle_) Load();

  if (av_frame_set_pkt_duration_)
    av_frame_set_pkt_duration_(frame, val);
  else {
    d()->AVFrameSetPktDuration(frame, val);
  }
}

int64_t AvcModuleProvider::av_frame_get_pkt_pos(const AVFrame *frame) {
  if (!avutil_handle_) Load();

  if (av_frame_get_pkt_pos_)
    return av_frame_get_pkt_pos_(frame);
  else {
    return d()->AVFrameGetPktPos(frame);
  }
}

void AvcModuleProvider::av_frame_set_pkt_pos(AVFrame *frame, int64_t val) {
  if (!avutil_handle_) Load();

  if (av_frame_set_pkt_pos_)
    av_frame_set_pkt_pos_(frame, val);
  else {
    d()->AVFrameSetPktPos(frame, val);
  }
}

int AvcModuleProvider::av_frame_get_sample_rate(const AVFrame *frame) {
  if (!avutil_handle_) Load();

  if (av_frame_get_sample_rate_)
    return av_frame_get_sample_rate_(frame);
  else {
    return d()->AVFrameGetSampleRate(frame);
  }
}

void AvcModuleProvider::av_frame_set_sample_rate(AVFrame *frame, int val) {
  if (!avutil_handle_) Load();

  if (av_frame_set_sample_rate_)
    av_frame_set_sample_rate_(frame, val);
  else {
    d()->AVFrameSetSampleRate(frame, val);
  }
}

void AvcModuleProvider::av_free(void *ptr) {
  if (!avutil_handle_) Load();
  av_free_(ptr);
}

void AvcModuleProvider::av_freep(void *ptr) {
  if (!avutil_handle_) Load();
  av_freep_(ptr);
}

int AvcModuleProvider::av_get_bytes_per_sample(int sample_fmt) {
  if (!avutil_handle_) Load();
  return av_get_bytes_per_sample_(sample_fmt);
}

int AvcModuleProvider::av_image_copy_to_buffer(uint8_t *dst, int dst_size,
                                               const uint8_t *const src_data[4],
                                               const int src_linesize[4], int pix_fmt,
                                               int width, int height, int align) {
  if (!avutil_handle_) Load();
  return av_image_copy_to_buffer_(dst, dst_size, src_data, src_linesize, pix_fmt, width,
                                  height, align);
}

int AvcModuleProvider::av_image_fill_arrays(uint8_t *dst_data[4], int dst_linesize[4],
                                            const uint8_t *src, int pix_fmt, int width,
                                            int height, int align) {
  if (!avutil_handle_) Load();
  return av_image_fill_arrays_(dst_data, dst_linesize, src, pix_fmt, width, height,
                               align);
}

int AvcModuleProvider::av_image_get_buffer_size(int pix_fmt, int width, int height,
                                                int align) {
  if (!avutil_handle_) Load();
  return av_image_get_buffer_size_(pix_fmt, width, height, align);
}

void AvcModuleProvider::av_log_default_callback(void *avcl, int level, const char *fmt,
                                                va_list vl) {
  if (!avutil_handle_) Load();
  return av_log_default_callback_(avcl, level, fmt, vl);
}

void AvcModuleProvider::av_log_set_callback(void (*callback)(void *, int, const char *,
                                                             va_list)) {
  if (!avutil_handle_) Load();
  return av_log_set_callback_(callback);
}

void AvcModuleProvider::av_log_set_level(int level) {
  if (!avutil_handle_) Load();
  return av_log_set_level_(level);
}

void *AvcModuleProvider::av_malloc(size_t size) {
  if (!avutil_handle_) Load();
  return av_malloc_(size);
}

AVBufferRef *AvcModuleProvider::av_buffer_create(uint8_t *data, int size,
                                                 void (*free)(void *opaque,
                                                              uint8_t *data),
                                                 void *opaque, int flags) {
  if (!avutil_handle_) Load();
  return av_buffer_create_(data, size, free, opaque, flags);
}

int AvcModuleProvider::av_buffer_is_writable(const AVBufferRef *buf) {
  if (!avutil_handle_) Load();
  return av_buffer_is_writable_(buf);
}

void *AvcModuleProvider::av_buffer_get_opaque(const AVBufferRef *buf) {
  if (!avutil_handle_) Load();
  return av_buffer_get_opaque_(buf);
}

int AvcModuleProvider::av_buffer_get_ref_count(const AVBufferRef *buf) {
  if (!avutil_handle_) Load();
  return av_buffer_get_ref_count_(buf);
}

int AvcModuleProvider::av_buffer_make_writable(AVBufferRef **buf) {
  if (!avutil_handle_) Load();
  return av_buffer_make_writable_(buf);
}

int AvcModuleProvider::av_buffer_realloc(AVBufferRef **pbuf, int size) {
  if (!avutil_handle_) Load();
  return av_buffer_realloc_(pbuf, size);
}

void AvcModuleProvider::av_buffer_unref(AVBufferRef **buf) {
  if (!avutil_handle_) Load();
  av_buffer_unref_(buf);
}

AVBufferRef *AvcModuleProvider::av_buffer_ref(AVBufferRef *buf) {
  if (!avutil_handle_) Load();
  return av_buffer_ref_(buf);
}

int AvcModuleProvider::av_pix_fmt_count_planes(int pix_fmt) {
  if (!avutil_handle_) Load();
  return av_pix_fmt_count_planes_(pix_fmt);
}

const AVPixFmtDescriptor* AvcModuleProvider::av_pix_fmt_desc_get(int /*enum AVPixelFormat*/ pix_fmt) {
  if (!avutil_handle_) Load();
  return av_pix_fmt_desc_get_(pix_fmt);
}

int AvcModuleProvider::av_sample_fmt_is_planar(int sample_fmt) {
  if (!avutil_handle_) Load();
  return av_sample_fmt_is_planar_(sample_fmt);
}

int AvcModuleProvider::av_samples_set_silence(uint8_t **audio_data, int offset,
                                              int nb_samples, int nb_channels,
                                              int sample_fmt) {
  if (!avutil_handle_) Load();
  return av_samples_set_silence_(audio_data, offset, nb_samples, nb_channels, sample_fmt);
}

int AvcModuleProvider::av_strerror(int errnum, char *errbuf, size_t errbuf_size) {
  if (!avutil_handle_) Load();
  return av_strerror_(errnum, errbuf, errbuf_size);
}

uint64_t AvcModuleProvider::av_get_channel_layout(const char *name) {
  if (!avutil_handle_) Load();
  return av_get_channel_layout_(name);
}

int AvcModuleProvider::av_get_channel_layout_nb_channels(uint64_t channel_layout) {
  if (!avutil_handle_) Load();
  return av_get_channel_layout_nb_channels_(channel_layout);
}

int64_t AvcModuleProvider::av_get_default_channel_layout(int nb_channels) {
  if (!avutil_handle_) Load();
  if (av_get_default_channel_layout_ == nullptr)
    return 0;

  return av_get_default_channel_layout_(nb_channels);
}

int AvcModuleProvider::av_get_channel_layout_channel_index(uint64_t channel_layout,
                                                           uint64_t channel) {
  if (!avutil_handle_) Load();
  return av_get_channel_layout_channel_index_(channel_layout, channel);
}

uint64_t AvcModuleProvider::av_channel_layout_extract_channel(uint64_t channel_layout,
                                                              int index) {
  if (!avutil_handle_) Load();
  return av_channel_layout_extract_channel_(channel_layout, index);
}

const char *AvcModuleProvider::av_get_channel_name(uint64_t channel) {
  if (!avutil_handle_) Load();
  return av_get_channel_name_(channel);
}

const char *AvcModuleProvider::av_get_channel_description(uint64_t channel) {
  if (!avutil_handle_) Load();
  return av_get_channel_description_(channel);
}

int AvcModuleProvider::av_get_standard_channel_layout(unsigned index, uint64_t *layout,
                                                      const char **name) {
  if (!avutil_handle_) Load();
  return av_get_standard_channel_layout_(index, layout, name);
}

// swscale
unsigned AvcModuleProvider::swscale_version() {
  if (!swscale_handle_) Load();
  return swscale_version_();
}

void AvcModuleProvider::sws_freeContext(struct SwsContext *swsContext) {
  return sws_freeContext_(swsContext);
}

struct SwsContext *AvcModuleProvider::sws_getContext(int srcW, int srcH, int srcFormat,
                                                     int dstW, int dstH, int dstFormat,
                                                     int flags, SwsFilter *srcFilter,
                                                     SwsFilter *dstFilter,
                                                     const double *param) {
  if (!swscale_handle_) Load();
  return sws_getContext_(srcW, srcH, srcFormat, dstW, dstH,
                         dstFormat, flags, srcFilter,
                         dstFilter, param);
}

int AvcModuleProvider::sws_scale(struct SwsContext *c, const uint8_t *const srcSlice[],
                                 const int srcStride[], int srcSliceY, int srcSliceH,
                                 uint8_t *const dst[], const int dstStride[]) {
  if (!swscale_handle_) Load();
  return sws_scale_(c, srcSlice, srcStride, srcSliceY, srcSliceH, dst, dstStride);
}

// swresample
unsigned AvcModuleProvider::swresample_version() {
  if (!swresample_handle_) Load();
  return swresample_version_();
}

SwrContext *AvcModuleProvider::swr_alloc() {
  if (!swresample_handle_) Load();
  return swr_alloc_();
}

int AvcModuleProvider::swr_init(SwrContext *s) {
  if (!swresample_handle_) Load();
  return swr_init_(s);
}
int AvcModuleProvider::swr_is_initialized(SwrContext *s) {
  if (!swresample_handle_) Load();
  return swr_is_initialized_(s);
}
void AvcModuleProvider::swr_free(SwrContext **s) {
  if (!swresample_handle_) Load();
  swr_free_(s);
}
void AvcModuleProvider::swr_close(SwrContext *s) {
  if (!swresample_handle_) Load();
  swr_close_(s);
}
int AvcModuleProvider::swr_convert(SwrContext *s, uint8_t **out, int out_count,
                                   const uint8_t **in, int in_count) {
  if (!swresample_handle_) Load();
  return swr_convert_(s, out, out_count, in, in_count);
}

int64_t AvcModuleProvider::swr_get_delay(struct SwrContext *s, int64_t base) {
  if (!swresample_handle_) Load();
  return swr_get_delay_(s, base);
}

// avdevice

unsigned AvcModuleProvider::avdevice_version() {
  if (!avdevice_handle_) Load();
  return avdevice_version_();
}

void AvcModuleProvider::avdevice_register_all() {
  if (!avdevice_handle_) Load();
  avdevice_register_all_();
}

std::shared_ptr<IAvcModuleDataWrapper> AvcModuleProvider::d() const {
  return data_wrapper_;
}

std::shared_ptr<IAvcVideoPixelFormatConverter> AvcModuleProvider::GetVideoPixelFormatConverter() {
  auto video_pixel_format_converter = video_pixel_format_converter_;

  if (video_pixel_format_converter)
    return video_pixel_format_converter;

  video_pixel_format_converter = avc::CreateAvcPixelFormatConverter(this);
  video_pixel_format_converter_ = video_pixel_format_converter;
  return video_pixel_format_converter;
}

}  // namespace detail
}  // namespace avc
