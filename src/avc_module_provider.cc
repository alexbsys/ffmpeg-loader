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

#ifndef DEBUG_PRINT
#define DEBUG_PRINT 0
#endif //DEBUG_PRINT

#ifndef FFMPEG_LOADER_DLL
#define FFMPEG_LOADER_DLL 0
#endif //FFMPEG_LOADER_DLL

#if FFMPEG_LOADER_DLL
#include <tools/api/dynamic_export.h>
#else //
#define API_EXPORT
#endif //FFMPEG_LOADER_DLL

#include "avc_module_provider.h"
#include "avc_pixel_format_converter.h"
#include "avc_default_libraries_names.hpp"
#include "avc_dynamic_library_finder.hpp"
#include "avc_dynamic_modules_loader.h"
#include "i_avc_module_data_wrapper_factory.h"

#include <avc/ffmpeg-loader.h>

#if DEBUG_PRINT
#include <cstdio>
#endif //DEBUG_PRINT

#if !defined(AVC_LIBRARIES_STATIC_LINK) || AVC_LIBRARIES_STATIC_LINK==0
#include "dynamic_loader.hpp"
#endif //AVC_LIBRARIES_STATIC_LINK

#include <cstdio>
#include <cstdlib>

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

// Макрос для проверки загруженности функции перед вызовом
// Использование: AVC_CHECK_AND_CALL(func_ptr, func_name, module_name)
#define AVC_CHECK_AND_CALL(func_ptr, func_name, module_name) \
  do { \
    if (!(func_ptr)) { \
      if (load_handler_) { \
        load_handler_->OnModuleFunctionsNotFound(this, module_name, func_name); \
      } \
      fprintf(stderr, "FATAL ERROR: Function '%s' from module '%s' was not loaded. " \
                     "Application cannot continue.\n", \
              func_name, module_name); \
      fflush(stderr); \
      std::abort(); \
    } \
  } while(0)

static std::string join_path(const std::string& base, const std::string& file) {
  if (base.empty())
    return file;
  if (file.empty())
    return base;
  
#ifdef _WIN32
  const char separator = '\\';
#else
  const char separator = '/';
#endif
  
  // Remove trailing separator from base if present
  std::string result = base;
  if (!result.empty() && (result.back() == '/' || result.back() == '\\')) {
    result.pop_back();
  }
  
  // Add file
  result += separator;
  result += file;
  
  return result;
}

static const char* kAvCodecModuleName = "avcodec";
static const char* kAvFormatModuleName = "avformat";
static const char* kAvUtilModuleName = "avutil";
static const char* kAvDeviceModuleName = "avdevice";
static const char* kSwScaleModuleName = "swscale";
static const char* kSwResampleModuleName = "swresample";


AvcModuleProvider::AvcModuleProvider(
  std::shared_ptr<cmf::IDynamicModulesLoader> modules_loader, 
  std::shared_ptr<IAvcModuleLoadHandler> load_handler, 
  const std::string &modules_path)
    : modules_loader_(modules_loader)
    , load_handler_(load_handler)
    , modules_path_(modules_path)
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

const std::string& AvcModuleProvider::GetAvCodecModulePath() const {
  return loaded_avcodec_module_name_;
}

const std::string& AvcModuleProvider::GetAvFormatModulePath() const {
  return loaded_avformat_module_name_;
}

const std::string& AvcModuleProvider::GetAvUtilModulePath() const {
  return loaded_avutil_module_name_;
}

const std::string& AvcModuleProvider::GetAvDeviceModulePath() const {
  return loaded_avdevice_module_name_;
}

const std::string& AvcModuleProvider::GetSwScaleModulePath() const {
  return loaded_swscale_module_name_;
}

const std::string& AvcModuleProvider::GetSwResampleModulePath() const {
  return loaded_swresample_module_name_;
}

int AvcModuleProvider::GetLibrariesCompatibilityScore() const {
  return data_wrapper_compatibility_score_;
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

bool AvcModuleProvider::SetupDataWrapper() {
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
  int selected_factory_score = 0;

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
    bool found_valid_factory = false;

    for (size_t i=0; i<scores.size(); i++) {
      if (scores[i] >= 0 && scores[i] < min_score) {
        min_score = scores[i];
        min_score_idx = i;
        found_valid_factory = true;
      }
    }

    if (!found_valid_factory) {
      // No valid factory found
      if (load_handler_) {
        load_handler_->OnModuleLoadError(this, "DATASTRUCT");
      }
      return false;
    }

    selected_factory = g_ffmpeg_data_wrappers[min_score_idx];
    selected_factory_score = min_score;
  }

  if (!selected_factory) {
#if DEBUG_PRINT
    printf("LIBAV data wrapper was not selected!");
#endif //DEBUG_PRINT
    if (load_handler_) {
      load_handler_->OnModuleLoadError(this, "DATASTRUCT");
    }
    return false;
  } 

  std::shared_ptr<IAvcModuleDataWrapper> data_wrapper = selected_factory->Create(shared_from_this());
  data_wrapper_ = data_wrapper;
  data_wrapper_compatibility_score_ = selected_factory_score;
  return true;
}

bool AvcModuleProvider::LoadAvModule(
  const char* name, 
  void** handle, 
  const std::string& module_name, 
  const std::string& noversion_module_name,
  bool enable_search_any_version,
  std::string& actual_loaded_module) {
  if (*handle != nullptr)
    return true;

  if (modules_path_.size()) {
    std::string path = join_path(modules_path_, module_name);
    actual_loaded_module = path;
    *handle = modules_loader_->LoadModule(actual_loaded_module);

    if (*handle == nullptr && noversion_module_name.size()) {
      std::string path = join_path(modules_path_, noversion_module_name);
      actual_loaded_module = path;
      *handle = modules_loader_->LoadModule(actual_loaded_module);
    }

    if (*handle == nullptr && enable_search_any_version) {
      auto lib_files = AvcDynamicLibraryFinder::FindLibraryFiles(modules_path_, name);
      if (lib_files.size()) {
        std::string full_path = join_path(modules_path_, lib_files[0]);
        actual_loaded_module = full_path;
        *handle = modules_loader_->LoadModule(actual_loaded_module);
      }
    }
  } 

  if (*handle == nullptr) {
    actual_loaded_module = module_name;
    *handle = modules_loader_->LoadModule(module_name);

    if (*handle == nullptr && noversion_module_name.size()) {
      actual_loaded_module = noversion_module_name;
      *handle = modules_loader_->LoadModule(noversion_module_name);
    }

    if (*handle == nullptr && enable_search_any_version) {
      auto lib_files = AvcDynamicLibraryFinder::FindLibraryFiles(modules_loader_->GetCurrentExecutableDir(), name);
      if (lib_files.size()) {
        std::string full_path = join_path(modules_loader_->GetCurrentExecutableDir(), lib_files[0]);
        actual_loaded_module = full_path;
        *handle = modules_loader_->LoadModule(actual_loaded_module);
      }
    }
  }

  if (*handle != nullptr) {
    return true;
  }
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

  std::string actual_module_path;

  if (!avcodec_handle_) {
    if (LoadAvModule(kAvCodecModuleName, &avcodec_handle_, avcodec_module_name_, strict_modules_names_ ? std::string() : kNoVersionAvCodecModuleName, 
      !strict_modules_names_, actual_module_path)) {

      LoadAvCodecFunctions();
      loaded_avcodec_module_name_ = actual_module_path;
    } else {
#if DEBUG_PRINT
      printf("cannot load module %s: %s, path %s", kAvCodecModuleName, avcodec_module_name_.c_str(), modules_path_.c_str());
#endif //DEBUG_PRINT
      if (load_handler_) {
        load_handler_->OnModuleLoadError(this, kAvCodecModuleName);
      }
    }
  }

  if (!avformat_handle_) {
    if (LoadAvModule(kAvFormatModuleName, &avformat_handle_, avformat_module_name_, strict_modules_names_ ? std::string() : kNoVersionAvFormatModuleName, 
      !strict_modules_names_, actual_module_path)) {

      LoadAvFormatFunctions();
      loaded_avformat_module_name_ = actual_module_path;
    } else {
#if DEBUG_PRINT
      printf("cannot load module %s: %s, path %s", kAvFormatModuleName, avformat_module_name_.c_str(), modules_path_.c_str());
#endif //DEBUG_PRINT
      if (load_handler_) {
        load_handler_->OnModuleLoadError(this, kAvFormatModuleName);
      }
    }
  }

  if (!avutil_handle_) {
    if (LoadAvModule(kAvUtilModuleName, &avutil_handle_, avutil_module_name_, strict_modules_names_ ? std::string() : kNoVersionAvUtilModuleName, 
      !strict_modules_names_, actual_module_path)) {

      LoadAvUtilFunctions();
      loaded_avutil_module_name_ = actual_module_path;
    } else {
#if DEBUG_PRINT
      printf("cannot load module %s: %s, path %s", kAvUtilModuleName, avutil_module_name_.c_str(), modules_path_.c_str());
#endif //DEBUG_PRINT

      if (load_handler_) {
        load_handler_->OnModuleLoadError(this, kAvUtilModuleName);
      }
    }
  }

  if (!avdevice_handle_) {
    if (LoadAvModule(kAvDeviceModuleName, &avdevice_handle_, avdevice_module_name_, strict_modules_names_ ? std::string() : kNoVersionAvDeviceModuleName, 
      !strict_modules_names_, actual_module_path)) {

      LoadAvDeviceFunctions();
      loaded_avdevice_module_name_ = actual_module_path;
    } else {
#if DEBUG_PRINT
      printf("cannot load module %s: %s, path %s", kAvDeviceModuleName, avdevice_module_name_.c_str(), modules_path_.c_str());
#endif //DEBUG_PRINT

      if (load_handler_) {
        load_handler_->OnModuleLoadError(this, kAvDeviceModuleName);
      }
    }
  }

  if (!swscale_handle_) {
    if (LoadAvModule(kSwScaleModuleName, &swscale_handle_, swscale_module_name_, strict_modules_names_ ? std::string() : kNoVersionSwScaleModuleName, 
      !strict_modules_names_, actual_module_path)) {

      LoadSwScaleFunctions();
      loaded_swscale_module_name_ = actual_module_path;
    } else {
#if DEBUG_PRINT
      printf("cannot load module %s: %s, path %s", kSwScaleModuleName, swscale_module_name_.c_str(), modules_path_.c_str());
#endif //DEBUG_PRINT

      if (load_handler_) {
        load_handler_->OnModuleLoadError(this, kSwScaleModuleName);
      }
    }
  }

  if (!swresample_handle_) {
    if (LoadAvModule(kSwResampleModuleName, &swresample_handle_, swresample_module_name_, strict_modules_names_ ? std::string() : kNoVersionSwResampleModuleName, 
      !strict_modules_names_, actual_module_path)) {

      LoadSwResampleFuctions();
      loaded_swresample_module_name_ = actual_module_path;
    } else {
#if DEBUG_PRINT
      printf("cannot load module %s: %s, path %s", kSwResampleModuleName, swresample_module_name_.c_str(), modules_path_.c_str());
#endif //DEBUG_PRINT

      if (load_handler_) {
        load_handler_->OnModuleLoadError(this, kSwResampleModuleName);
      }
    }
  }

  if (SetupDataWrapper()) {
    if (load_handler_) {
      load_handler_->OnLoadFinished(this);
    }
  }
}

void AvcModuleProvider::Unload() {
  if (load_handler_) {
    load_handler_->OnBeforeUnload(this);
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
    .LoadProc("av_packet_rescale_ts", av_packet_rescale_ts_)
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
    .LoadProc("avcodec_configuration", avcodec_configuration_)
    .LoadProc("avcodec_license", avcodec_license_)
    .LoadProc("avcodec_get_class", avcodec_get_class_)
    .LoadProc("avsubtitle_free", avsubtitle_free_)
    .LoadProc("avcodec_align_dimensions", avcodec_align_dimensions_)
    .LoadProc("avcodec_align_dimensions2", avcodec_align_dimensions2_)
    .LoadProc("avcodec_enum_to_chroma_pos", avcodec_enum_to_chroma_pos_)
    .LoadProc("avcodec_chroma_pos_to_enum", avcodec_chroma_pos_to_enum_)
    .LoadProc("avcodec_decode_subtitle2", avcodec_decode_subtitle2_)
    .LoadProc("avcodec_get_hw_frames_parameters", avcodec_get_hw_frames_parameters_)
    .LoadProc("av_parser_init", av_parser_init_)
    .LoadProc("av_parser_iterate", av_parser_iterate_)
    .LoadProc("av_parser_parse2", av_parser_parse2_)
    .LoadProc("av_parser_close", av_parser_close_)
    .LoadProc("avcodec_encode_subtitle", avcodec_encode_subtitle_)
    .LoadProc("avcodec_pix_fmt_to_codec_tag", avcodec_pix_fmt_to_codec_tag_)
    .LoadProc("avcodec_find_best_pix_fmt_of_list", avcodec_find_best_pix_fmt_of_list_)
    .LoadProc("avcodec_default_get_format", avcodec_default_get_format_)
    .LoadProc("avcodec_fill_audio_frame", avcodec_fill_audio_frame_)
    .LoadProc("av_get_audio_frame_duration", av_get_audio_frame_duration_)
    .LoadProc("av_fast_padded_malloc", av_fast_padded_malloc_)
    .LoadProc("av_fast_padded_mallocz", av_fast_padded_mallocz_)
    .LoadProc("avcodec_is_open", avcodec_is_open_)
    ;

  if (avcodec_loader.HasErrors()) {
#if DEBUG_PRINT
    printf("avcodec functions were not loaded: %s", avcodec_loader.ProblemFunctions().str().c_str());
#endif //DEBUG_PRINT

    if (load_handler_) {
      load_handler_->OnModuleFunctionsNotFound(this, kAvCodecModuleName, avcodec_loader.ProblemFunctions().str().c_str());
    }
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
    .LoadProc("av_guess_sample_aspect_ratio", av_guess_sample_aspect_ratio_)
    .LoadProc("av_guess_frame_rate", av_guess_frame_rate_)
    .LoadProc("av_find_input_format", av_find_input_format_)
    .LoadProc("av_guess_format", av_guess_format_)
    .LoadProc("av_guess_codec", av_guess_codec_)
    .LoadProc("av_find_best_stream", av_find_best_stream_)
    .LoadProc("avformat_init_output", avformat_init_output_)
    .LoadProc("av_write_uncoded_frame", av_write_uncoded_frame_)
    .LoadProc("av_interleaved_write_uncoded_frame", av_interleaved_write_uncoded_frame_)
    .LoadProc("av_write_uncoded_frame_query", av_write_uncoded_frame_query_)
    .LoadProc("av_get_output_timestamp", av_get_output_timestamp_)
    .LoadProc("av_index_search_timestamp", av_index_search_timestamp_)
    .LoadProc("avformat_index_get_entries_count", avformat_index_get_entries_count_)
    .LoadProc("avformat_index_get_entry", avformat_index_get_entry_)
    .LoadProc("avformat_index_get_entry_from_timestamp", avformat_index_get_entry_from_timestamp_)
    .LoadProc("av_add_index_entry", av_add_index_entry_)
    .LoadProc("av_url_split", av_url_split_)
    .LoadProc("av_sdp_create", av_sdp_create_)
    .LoadProc("av_match_ext", av_match_ext_)
    .LoadProc("avformat_query_codec", avformat_query_codec_)
    .LoadProc("avformat_get_riff_video_tags", avformat_get_riff_video_tags_)
    .LoadProc("avformat_get_riff_audio_tags", avformat_get_riff_audio_tags_)
    .LoadProc("avformat_get_mov_video_tags", avformat_get_mov_video_tags_)
    .LoadProc("avformat_get_mov_audio_tags", avformat_get_mov_audio_tags_)
    .LoadProc("av_stream_get_codec_timebase", av_stream_get_codec_timebase_)
    .LoadProc("av_read_frame", av_read_frame_)
    .LoadProc("av_read_play", av_read_play_)
    .LoadProc("av_read_pause", av_read_pause_)
    .LoadProc("av_register_all", av_register_all_)
    .LoadProc("avformat_flush", avformat_flush_)
    .LoadProc("avformat_seek_file", avformat_seek_file_)
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
#if DEBUG_PRINT
    printf("avformat functions were not loaded: %s", avformat_loader.ProblemFunctions().str().c_str());
#endif //DEBUG_PRINT

    if (load_handler_) {
      load_handler_->OnModuleFunctionsNotFound(this, kAvFormatModuleName, avformat_loader.ProblemFunctions().str().c_str());
    }
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
    .LoadProc("av_rescale_q_rnd", av_rescale_q_rnd_)
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
    .LoadProc("av_frame_ref", av_frame_ref_)
    .LoadProc("av_frame_replace", av_frame_replace_)
    .LoadProc("av_frame_clone", av_frame_clone_)
    .LoadProc("av_frame_unref", av_frame_unref_)
    .LoadProc("av_frame_move_ref", av_frame_move_ref_)
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
#if DEBUG_PRINT
    printf("avutil functions were not loaded: %s", avutil_loader.ProblemFunctions().str().c_str());
#endif //DEBUG_PRINT

    if (load_handler_) {
      load_handler_->OnModuleFunctionsNotFound(this, kAvUtilModuleName, avutil_loader.ProblemFunctions().str().c_str());
    }
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
#if DEBUG_PRINT
    printf("swscale functions were not loaded: %s", swscale_loader.ProblemFunctions().str().c_str());
#endif //DEBUG_PRINT

    if (load_handler_) {
      load_handler_->OnModuleFunctionsNotFound(this, kSwScaleModuleName, swscale_loader.ProblemFunctions().str().c_str());
    }
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
#if DEBUG_PRINT
    printf("swresample functions were not loaded: %s", swresample_loader.ProblemFunctions().str().c_str());
#endif //DEBUG_PRINT

    if (load_handler_) {
      load_handler_->OnModuleFunctionsNotFound(this, kSwResampleModuleName, swresample_loader.ProblemFunctions().str().c_str());
    }
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
#if DEBUG_PRINT
    printf("avdevice functions were not loaded: %s", avdevice_loader.ProblemFunctions().str().c_str());
#endif //DEBUG_PRINT

    if (load_handler_) {
      load_handler_->OnModuleFunctionsNotFound(this, kAvDeviceModuleName, avdevice_loader.ProblemFunctions().str().c_str());
    }

  }
#endif //AV_LIBRARIES_STATIC_LINK
}

// avcodec
unsigned AvcModuleProvider::avcodec_version() {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_version_, "avcodec_version", kAvCodecModuleName);
  return avcodec_version_();
}

int AvcModuleProvider::avcodec_encode_video2(AVCodecContext *avctx, AVPacket *avpkt,
                                             const AVFrame *frame, int *got_packet_ptr) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_encode_video2_, "avcodec_encode_video2", kAvCodecModuleName);
  return avcodec_encode_video2_(avctx, avpkt, frame, got_packet_ptr);
}

size_t AvcModuleProvider::av_get_codec_tag_string(char *buf, size_t buf_size,
                                                  unsigned int codec_tag) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_get_codec_tag_string_, "av_get_codec_tag_string", kAvCodecModuleName);
  return av_get_codec_tag_string_(buf, buf_size, codec_tag);
}

int AvcModuleProvider::avcodec_parameters_copy(AVCodecParameters *dst,
                                               const AVCodecParameters *src) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_parameters_copy_, "avcodec_parameters_copy", kAvCodecModuleName);
  return avcodec_parameters_copy_(dst, src);
}

int AvcModuleProvider::avcodec_parameters_from_context(AVCodecParameters *par,
                                                       const AVCodecContext *codec) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_parameters_from_context_, "avcodec_parameters_from_context", kAvCodecModuleName);
  return avcodec_parameters_from_context_(par, codec);
}

void AvcModuleProvider::avcodec_parameters_free(AVCodecParameters **par) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_parameters_free_, "avcodec_parameters_free", kAvCodecModuleName);
  avcodec_parameters_free_(par);
}

AVCodecParameters *AvcModuleProvider::avcodec_parameters_alloc() {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_parameters_alloc_, "avcodec_parameters_alloc", kAvCodecModuleName);
  return avcodec_parameters_alloc_();
}

int AvcModuleProvider::avcodec_parameters_to_context(AVCodecContext *codec,
                                                     const AVCodecParameters *par) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_parameters_to_context_, "avcodec_parameters_to_context", kAvCodecModuleName);
  return avcodec_parameters_to_context_(codec, par);
}

void AvcModuleProvider::av_register_hwaccel(AVHWAccel *hwaccel) {
  if (!avcodec_handle_) Load();
  if (av_register_hwaccel_)
    av_register_hwaccel_(hwaccel);
}

AVHWAccel *AvcModuleProvider::av_hwaccel_next(const AVHWAccel *hwaccel) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwaccel_next_, "av_hwaccel_next", kAvCodecModuleName);
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
  AVC_CHECK_AND_CALL(av_packet_alloc_, "av_packet_alloc", kAvCodecModuleName);
  return av_packet_alloc_();
}

AVPacket *AvcModuleProvider::av_packet_clone(AVPacket *src) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_packet_clone_, "av_packet_clone", kAvCodecModuleName);
  return av_packet_clone_(src);
}

void AvcModuleProvider::av_packet_free(AVPacket **pkt) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_packet_free_, "av_packet_free", kAvCodecModuleName);
  return av_packet_free_(pkt);
}

void AvcModuleProvider::av_init_packet(AVPacket *pkt) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_init_packet_, "av_init_packet", kAvCodecModuleName);
  av_init_packet_(pkt);
}

int AvcModuleProvider::av_new_packet(AVPacket *pkt, int size) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_new_packet_, "av_new_packet", kAvCodecModuleName);
  return av_new_packet_(pkt, size);
}

void AvcModuleProvider::av_packet_ref(AVPacket *dst, const AVPacket* src) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_packet_ref_, "av_packet_ref", kAvCodecModuleName);
  av_packet_ref_(dst, src);
}

void AvcModuleProvider::av_packet_unref(AVPacket *pkt) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_packet_unref_, "av_packet_unref", kAvCodecModuleName);
  av_packet_unref_(pkt);
}

void AvcModuleProvider::av_packet_rescale_ts(AVPacket* pkt, cmf::MediaTimeBase tb_src, cmf::MediaTimeBase tb_dst) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_packet_rescale_ts_, "av_packet_rescale_ts", kAvCodecModuleName);
  av_packet_rescale_ts_(pkt, { tb_src.num_,tb_src.den_ }, { tb_dst.num_, tb_dst.den_ });
}

AVCodecContext *AvcModuleProvider::avcodec_alloc_context3(const AVCodec *codec) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_alloc_context3_, "avcodec_alloc_context3", kAvCodecModuleName);
  return avcodec_alloc_context3_(codec);
}

void AvcModuleProvider::avcodec_free_context(AVCodecContext **avctx) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_free_context_, "avcodec_free_context", kAvCodecModuleName);
  avcodec_free_context_(avctx);
}

AVCodec *AvcModuleProvider::avcodec_find_decoder(int id) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_find_decoder_, "avcodec_find_decoder", kAvCodecModuleName);
  return avcodec_find_decoder_(id);
}
AVCodec *AvcModuleProvider::avcodec_find_decoder_by_name(const char *name) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_find_decoder_by_name_, "avcodec_find_decoder_by_name", kAvCodecModuleName);
  return avcodec_find_decoder_by_name_(name);
}

AVCodec *AvcModuleProvider::avcodec_find_encoder(int /*enum AVCodecID*/ id) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_find_encoder_, "avcodec_find_encoder", kAvCodecModuleName);
  return avcodec_find_encoder_(id);
}

AVCodec *AvcModuleProvider::avcodec_find_encoder_by_name(const char *name) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_find_encoder_by_name_, "avcodec_find_encoder_by_name", kAvCodecModuleName);
  return avcodec_find_encoder_by_name_(name);
}
void AvcModuleProvider::avcodec_flush_buffers(AVCodecContext *avctx) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_flush_buffers_, "avcodec_flush_buffers", kAvCodecModuleName);
  avcodec_flush_buffers_(avctx);
}

const char *AvcModuleProvider::avcodec_get_name(int /*enum AVCodecID*/ id) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_get_name_, "avcodec_get_name", kAvCodecModuleName);
  return avcodec_get_name_(id);
}
int AvcModuleProvider::av_codec_is_encoder(const AVCodec *codec) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_codec_is_encoder_, "av_codec_is_encoder", kAvCodecModuleName);
  return av_codec_is_encoder_(codec);
}
int AvcModuleProvider::av_codec_is_decoder(const AVCodec *codec) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_codec_is_decoder_, "av_codec_is_decoder", kAvCodecModuleName);
  return av_codec_is_decoder_(codec);
}
int AvcModuleProvider::avcodec_open2(AVCodecContext *avctx, const AVCodec *codec,
                                     AVDictionary **options) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_open2_, "avcodec_open2", kAvCodecModuleName);
  return avcodec_open2_(avctx, codec, options);
}
int AvcModuleProvider::avcodec_receive_frame(AVCodecContext *avctx, AVFrame *frame) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_receive_frame_, "avcodec_receive_frame", kAvCodecModuleName);
  return avcodec_receive_frame_(avctx, frame);
}
int AvcModuleProvider::avcodec_send_frame(AVCodecContext *avctx, const AVFrame *frame) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_send_frame_, "avcodec_send_frame", kAvCodecModuleName);
  return avcodec_send_frame_(avctx, frame);
}
int AvcModuleProvider::avcodec_receive_packet(AVCodecContext *avctx, AVPacket *avpkt) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_receive_packet_, "avcodec_receive_packet", kAvCodecModuleName);
  return avcodec_receive_packet_(avctx, avpkt);
}

int AvcModuleProvider::avcodec_send_packet(AVCodecContext *avctx, const AVPacket *avpkt) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_send_packet_, "avcodec_send_packet", kAvCodecModuleName);
  return avcodec_send_packet_(avctx, avpkt);
}

void AvcModuleProvider::avcodec_register_all() {
  if (!avcodec_handle_) Load();
  if (avcodec_register_all_)
    avcodec_register_all_();
}

const char *AvcModuleProvider::avcodec_configuration(void) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_configuration_, "avcodec_configuration", kAvCodecModuleName);
  return avcodec_configuration_();
}

const char *AvcModuleProvider::avcodec_license(void) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_license_, "avcodec_license", kAvCodecModuleName);
  return avcodec_license_();
}

const AVClass *AvcModuleProvider::avcodec_get_class(void) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_get_class_, "avcodec_get_class", kAvCodecModuleName);
  return avcodec_get_class_();
}

void AvcModuleProvider::avsubtitle_free(AVSubtitle *sub) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avsubtitle_free_, "avsubtitle_free", kAvCodecModuleName);
  avsubtitle_free_(sub);
}

int AvcModuleProvider::avcodec_align_dimensions(AVCodecContext *s, int *width, int *height) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_align_dimensions_, "avcodec_align_dimensions", kAvCodecModuleName);
  return avcodec_align_dimensions_(s, width, height);
}

int AvcModuleProvider::avcodec_align_dimensions2(AVCodecContext *s, int *width, int *height, int linesize_align[8]) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_align_dimensions2_, "avcodec_align_dimensions2", kAvCodecModuleName);
  return avcodec_align_dimensions2_(s, width, height, linesize_align);
}

int AvcModuleProvider::avcodec_enum_to_chroma_pos(int *xpos, int *ypos, int /*enum AVChromaLocation*/ pos) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_enum_to_chroma_pos_, "avcodec_enum_to_chroma_pos", kAvCodecModuleName);
  return avcodec_enum_to_chroma_pos_(xpos, ypos, pos);
}

int AvcModuleProvider::avcodec_chroma_pos_to_enum(int *xpos, int *ypos, int /*enum AVChromaLocation*/ pos) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_chroma_pos_to_enum_, "avcodec_chroma_pos_to_enum", kAvCodecModuleName);
  return avcodec_chroma_pos_to_enum_(xpos, ypos, pos);
}

int AvcModuleProvider::avcodec_decode_subtitle2(AVCodecContext *avctx, AVSubtitle *sub, int *got_sub_ptr, const AVPacket *avpkt) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_decode_subtitle2_, "avcodec_decode_subtitle2", kAvCodecModuleName);
  return avcodec_decode_subtitle2_(avctx, sub, got_sub_ptr, avpkt);
}

int AvcModuleProvider::avcodec_get_hw_frames_parameters(AVCodecContext *avctx, AVBufferRef *device_ref, const char *hw_pix_fmt, AVBufferRef **out_frames_ref) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_get_hw_frames_parameters_, "avcodec_get_hw_frames_parameters", kAvCodecModuleName);
  return avcodec_get_hw_frames_parameters_(avctx, device_ref, hw_pix_fmt, out_frames_ref);
}

AVCodecParserContext *AvcModuleProvider::av_parser_init(int /*enum AVCodecID*/ codec_id) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_parser_init_, "av_parser_init", kAvCodecModuleName);
  return av_parser_init_(codec_id);
}

const AVCodecParser *AvcModuleProvider::av_parser_iterate(void **opaque) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_parser_iterate_, "av_parser_iterate", kAvCodecModuleName);
  return av_parser_iterate_(opaque);
}

int AvcModuleProvider::av_parser_parse2(AVCodecParserContext *s, AVCodecContext *avctx, uint8_t **poutbuf, int *poutbuf_size, const uint8_t *buf, int buf_size, int64_t pts, int64_t dts, int64_t pos) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_parser_parse2_, "av_parser_parse2", kAvCodecModuleName);
  return av_parser_parse2_(s, avctx, poutbuf, poutbuf_size, buf, buf_size, pts, dts, pos);
}

void AvcModuleProvider::av_parser_close(AVCodecParserContext *s) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_parser_close_, "av_parser_close", kAvCodecModuleName);
  av_parser_close_(s);
}

int AvcModuleProvider::avcodec_encode_subtitle(AVCodecContext *avctx, uint8_t *buf, int buf_size, const AVSubtitle *sub) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_encode_subtitle_, "avcodec_encode_subtitle", kAvCodecModuleName);
  return avcodec_encode_subtitle_(avctx, buf, buf_size, sub);
}

unsigned int AvcModuleProvider::avcodec_pix_fmt_to_codec_tag(const AVPixFmtDescriptor *pix_fmt) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_pix_fmt_to_codec_tag_, "avcodec_pix_fmt_to_codec_tag", kAvCodecModuleName);
  return avcodec_pix_fmt_to_codec_tag_(pix_fmt);
}

int AvcModuleProvider::avcodec_find_best_pix_fmt_of_list(const int /*enum AVPixelFormat*/ *pix_fmt_list, int /*enum AVPixelFormat*/ src_pix_fmt, int has_alpha, int *loss_ptr) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_find_best_pix_fmt_of_list_, "avcodec_find_best_pix_fmt_of_list", kAvCodecModuleName);
  return avcodec_find_best_pix_fmt_of_list_(pix_fmt_list, src_pix_fmt, has_alpha, loss_ptr);
}

int AvcModuleProvider::avcodec_default_get_format(struct AVCodecContext *s, const int /*enum AVPixelFormat*/ *fmt) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_default_get_format_, "avcodec_default_get_format", kAvCodecModuleName);
  return avcodec_default_get_format_(s, fmt);
}

int AvcModuleProvider::avcodec_fill_audio_frame(AVFrame *frame, int nb_channels, int /*enum AVSampleFormat*/ sample_fmt, const uint8_t *buf, int buf_size, int align) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_fill_audio_frame_, "avcodec_fill_audio_frame", kAvCodecModuleName);
  return avcodec_fill_audio_frame_(frame, nb_channels, sample_fmt, buf, buf_size, align);
}

int AvcModuleProvider::av_get_audio_frame_duration(AVCodecContext *avctx, int frame_bytes) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_get_audio_frame_duration_, "av_get_audio_frame_duration", kAvCodecModuleName);
  return av_get_audio_frame_duration_(avctx, frame_bytes);
}

void AvcModuleProvider::av_fast_padded_malloc(void *ptr, unsigned int *size, size_t min_size) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_fast_padded_malloc_, "av_fast_padded_malloc", kAvCodecModuleName);
  av_fast_padded_malloc_(ptr, size, min_size);
}

void AvcModuleProvider::av_fast_padded_mallocz(void *ptr, unsigned int *size, size_t min_size) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_fast_padded_mallocz_, "av_fast_padded_mallocz", kAvCodecModuleName);
  av_fast_padded_mallocz_(ptr, size, min_size);
}

int AvcModuleProvider::avcodec_is_open(AVCodecContext *s) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_is_open_, "avcodec_is_open", kAvCodecModuleName);
  return avcodec_is_open_(s);
}

const AVCodec *AvcModuleProvider::av_codec_iterate(void **opaque) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(av_codec_iterate_, "av_codec_iterate", kAvCodecModuleName);
  return av_codec_iterate_(opaque);
}

const AVCodecHWConfig *AvcModuleProvider::avcodec_get_hw_config(const AVCodec *codec,
                                                                int index) {
  if (!avcodec_handle_) Load();
  AVC_CHECK_AND_CALL(avcodec_get_hw_config_, "avcodec_get_hw_config", kAvCodecModuleName);
  return avcodec_get_hw_config_(codec, index);
}
//// avformat

unsigned AvcModuleProvider::avformat_version() {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_version_, "avformat_version", kAvFormatModuleName);
  return avformat_version_();
}

void AvcModuleProvider::av_dump_format(AVFormatContext *ic, int index, const char *url,
                                       int is_output) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_dump_format_, "av_dump_format", kAvFormatModuleName);
  return av_dump_format_(ic, index, url, is_output);
}

cmf::MediaTimeBase AvcModuleProvider::av_guess_sample_aspect_ratio(AVFormatContext* ctx, AVStream* stream, AVFrame* frame) {
  if (!avformat_handle_) Load();
  if (!av_guess_sample_aspect_ratio_) return cmf::MediaTimeBase{ 0,0 };
  
  auto avr = av_guess_sample_aspect_ratio_(ctx, stream, frame);
  return cmf::MediaTimeBase(avr.num, avr.den);
}

cmf::MediaTimeBase AvcModuleProvider::av_guess_frame_rate(AVFormatContext* ctx, AVStream* stream, AVFrame* frame) {
  if (!avformat_handle_) Load();
  if (!av_guess_frame_rate_) return cmf::MediaTimeBase{ 0,0 };
  auto avr = av_guess_frame_rate_(ctx, stream, frame);
  return cmf::MediaTimeBase(avr.num, avr.den);
}

AVInputFormat *AvcModuleProvider::av_find_input_format(const char *short_name) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_find_input_format_, "av_find_input_format", kAvFormatModuleName);
  return av_find_input_format_(short_name);
}

AVOutputFormat *AvcModuleProvider::av_guess_format(const char *short_name,
                                                   const char *filename,
                                                   const char *mime_type) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_guess_format_, "av_guess_format", kAvFormatModuleName);
  return av_guess_format_(short_name, filename, mime_type);
}

int AvcModuleProvider::av_guess_codec(AVOutputFormat *fmt, const char *short_name, const char *filename,
                                     const char *mime_type, int /*enum AVMediaType*/ type) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_guess_codec_, "av_guess_codec", kAvFormatModuleName);
  return av_guess_codec_(fmt, short_name, filename, mime_type, type);
}

int AvcModuleProvider::av_find_best_stream(AVFormatContext *ic, int /*enum AVMediaType*/ type, int wanted_stream_nb, int related_stream, const AVCodec **decoder_ret, int flags) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_find_best_stream_, "av_find_best_stream", kAvFormatModuleName);
  return av_find_best_stream_(ic, type, wanted_stream_nb, related_stream, decoder_ret, flags);
}

int AvcModuleProvider::avformat_init_output(AVFormatContext *s, AVDictionary **options) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_init_output_, "avformat_init_output", kAvFormatModuleName);
  return avformat_init_output_(s, options);
}

int AvcModuleProvider::av_write_uncoded_frame(AVFormatContext *s, int stream_index, AVFrame *frame) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_write_uncoded_frame_, "av_write_uncoded_frame", kAvFormatModuleName);
  return av_write_uncoded_frame_(s, stream_index, frame);
}

int AvcModuleProvider::av_interleaved_write_uncoded_frame(AVFormatContext *s, int stream_index, AVFrame *frame) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_interleaved_write_uncoded_frame_, "av_interleaved_write_uncoded_frame", kAvFormatModuleName);
  return av_interleaved_write_uncoded_frame_(s, stream_index, frame);
}

int AvcModuleProvider::av_write_uncoded_frame_query(AVFormatContext *s, int stream_index) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_write_uncoded_frame_query_, "av_write_uncoded_frame_query", kAvFormatModuleName);
  return av_write_uncoded_frame_query_(s, stream_index);
}

int AvcModuleProvider::av_get_output_timestamp(AVFormatContext *s, int stream, int64_t *dts, int64_t *wall) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_get_output_timestamp_, "av_get_output_timestamp", kAvFormatModuleName);
  return av_get_output_timestamp_(s, stream, dts, wall);
}

int AvcModuleProvider::av_index_search_timestamp(AVStream *st, int64_t timestamp, int flags) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_index_search_timestamp_, "av_index_search_timestamp", kAvFormatModuleName);
  return av_index_search_timestamp_(st, timestamp, flags);
}

int AvcModuleProvider::avformat_index_get_entries_count(AVStream *st) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_index_get_entries_count_, "avformat_index_get_entries_count", kAvFormatModuleName);
  return avformat_index_get_entries_count_(st);
}

AVIndexEntry *AvcModuleProvider::avformat_index_get_entry(AVStream *st, int idx) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_index_get_entry_, "avformat_index_get_entry", kAvFormatModuleName);
  return avformat_index_get_entry_(st, idx);
}

AVIndexEntry *AvcModuleProvider::avformat_index_get_entry_from_timestamp(AVStream *st, int64_t wanted_timestamp, int flags) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_index_get_entry_from_timestamp_, "avformat_index_get_entry_from_timestamp", kAvFormatModuleName);
  return avformat_index_get_entry_from_timestamp_(st, wanted_timestamp, flags);
}

int AvcModuleProvider::av_add_index_entry(AVStream *st, int64_t pos, int64_t timestamp, int size, int distance, int flags) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_add_index_entry_, "av_add_index_entry", kAvFormatModuleName);
  return av_add_index_entry_(st, pos, timestamp, size, distance, flags);
}

void AvcModuleProvider::av_url_split(char *proto, int proto_size, char *authorization, int authorization_size, char *hostname, int hostname_size, int *port_ptr, char *path, int path_size, const char *url) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_url_split_, "av_url_split", kAvFormatModuleName);
  av_url_split_(proto, proto_size, authorization, authorization_size, hostname, hostname_size, port_ptr, path, path_size, url);
}

int AvcModuleProvider::av_sdp_create(AVFormatContext *ac[], int n_files, char *buf, int size) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_sdp_create_, "av_sdp_create", kAvFormatModuleName);
  return av_sdp_create_(ac, n_files, buf, size);
}

int AvcModuleProvider::av_match_ext(const char *filename, const char *extensions) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_match_ext_, "av_match_ext", kAvFormatModuleName);
  return av_match_ext_(filename, extensions);
}

int AvcModuleProvider::avformat_query_codec(const AVOutputFormat *ofmt, int /*enum AVCodecID*/ codec_id, int std_compliance) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_query_codec_, "avformat_query_codec", kAvFormatModuleName);
  return avformat_query_codec_(ofmt, codec_id, std_compliance);
}

const char *AvcModuleProvider::avformat_get_riff_video_tags(void) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_get_riff_video_tags_, "avformat_get_riff_video_tags", kAvFormatModuleName);
  return avformat_get_riff_video_tags_();
}

const char *AvcModuleProvider::avformat_get_riff_audio_tags(void) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_get_riff_audio_tags_, "avformat_get_riff_audio_tags", kAvFormatModuleName);
  return avformat_get_riff_audio_tags_();
}

const char *AvcModuleProvider::avformat_get_mov_video_tags(void) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_get_mov_video_tags_, "avformat_get_mov_video_tags", kAvFormatModuleName);
  return avformat_get_mov_video_tags_();
}

const char *AvcModuleProvider::avformat_get_mov_audio_tags(void) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_get_mov_audio_tags_, "avformat_get_mov_audio_tags", kAvFormatModuleName);
  return avformat_get_mov_audio_tags_();
}

AVRational AvcModuleProvider::av_stream_get_codec_timebase(const AVStream *st) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_stream_get_codec_timebase_, "av_stream_get_codec_timebase", kAvFormatModuleName);
  return av_stream_get_codec_timebase_(st);
}

int AvcModuleProvider::av_read_frame(AVFormatContext *s, AVPacket *pkt) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_read_frame_, "av_read_frame", kAvFormatModuleName);
  return av_read_frame_(s, pkt);
}

int AvcModuleProvider::av_read_play(AVFormatContext *s) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_read_play_, "av_read_play", kAvFormatModuleName);
  return av_read_play_(s);
}

int AvcModuleProvider::av_read_pause(AVFormatContext *s) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_read_pause_, "av_read_pause", kAvFormatModuleName);
  return av_read_pause_(s);
}

void AvcModuleProvider::av_register_all(void) {
  if (!avformat_handle_) Load();
  if (av_register_all_)
    av_register_all_();
}

int AvcModuleProvider::avformat_seek_file(AVFormatContext* s, int stream_index,
  int64_t min_ts, int64_t ts, int64_t max_ts, int flags) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_seek_file_, "avformat_seek_file", kAvFormatModuleName);
  return avformat_seek_file_(s, stream_index, min_ts, ts, max_ts, flags);
}

int AvcModuleProvider::avformat_flush(AVFormatContext *s) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_flush_, "avformat_flush", kAvFormatModuleName);
  return avformat_flush_(s);
}

int AvcModuleProvider::av_seek_frame(AVFormatContext *s, int stream_index,
                                     int64_t timestamp, int flags) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_seek_frame_, "av_seek_frame", kAvFormatModuleName);
  return av_seek_frame_(s, stream_index, timestamp, flags);
}

int AvcModuleProvider::av_write_frame(AVFormatContext *s, AVPacket *pkt) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_write_frame_, "av_write_frame", kAvFormatModuleName);
  return av_write_frame_(s, pkt);
}

int AvcModuleProvider::av_interleaved_write_frame(AVFormatContext *s, AVPacket *pkt) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_interleaved_write_frame_, "av_interleaved_write_frame", kAvFormatModuleName);
  return av_interleaved_write_frame_(s, pkt);
}

int AvcModuleProvider::av_write_trailer(AVFormatContext *s) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_write_trailer_, "av_write_trailer", kAvFormatModuleName);
  return av_write_trailer_(s);
}

AVFormatContext *AvcModuleProvider::avformat_alloc_context(void) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_alloc_context_, "avformat_alloc_context", kAvFormatModuleName);
  return avformat_alloc_context_();
}

int AvcModuleProvider::avformat_alloc_output_context2(AVFormatContext **ctx,
                                                      AVOutputFormat *oformat,
                                                      const char *format_name,
                                                      const char *filename) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_alloc_output_context2_, "avformat_alloc_output_context2", kAvFormatModuleName);
  return avformat_alloc_output_context2_(ctx, oformat, format_name, filename);
}

void AvcModuleProvider::avformat_free_context(AVFormatContext *s) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_free_context_, "avformat_free_context", kAvFormatModuleName);
  avformat_free_context_(s);
}

void AvcModuleProvider::avformat_close_input(AVFormatContext **s) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_close_input_, "avformat_close_input", kAvFormatModuleName);
  avformat_close_input_(s);
}

int AvcModuleProvider::avformat_find_stream_info(AVFormatContext *ic,
                                                 AVDictionary **options) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_find_stream_info_, "avformat_find_stream_info", kAvFormatModuleName);
  return avformat_find_stream_info_(ic, options);
}

int AvcModuleProvider::avformat_network_init(void) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_network_init_, "avformat_network_init", kAvFormatModuleName);
  return avformat_network_init_();
}

int AvcModuleProvider::avformat_network_deinit(void) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_network_deinit_, "avformat_network_deinit", kAvFormatModuleName);
  return avformat_network_deinit_();
}

AVStream *AvcModuleProvider::avformat_new_stream(AVFormatContext *s, const AVCodec *c) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_new_stream_, "avformat_new_stream", kAvFormatModuleName);
  return avformat_new_stream_(s, c);
}

int AvcModuleProvider::av_stream_add_side_data(AVStream *st, int type,
                                               uint8_t *data, size_t size) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_stream_add_side_data_, "av_stream_add_side_data", kAvFormatModuleName);
  return av_stream_add_side_data_(st, type, data, size);
}

uint8_t* AvcModuleProvider::av_stream_new_side_data(AVStream *stream,
                                                    int type, int size) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_stream_new_side_data_, "av_stream_new_side_data", kAvFormatModuleName);
  return av_stream_new_side_data_(stream, type, size);
}

uint8_t* AvcModuleProvider::av_stream_get_side_data(const AVStream *stream,
                                                    int type, int *size) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(av_stream_get_side_data_, "av_stream_get_side_data", kAvFormatModuleName);
  return av_stream_get_side_data_(stream, type, size);
}

int AvcModuleProvider::avformat_open_input(AVFormatContext **ps, const char *url,
                                           AVInputFormat *fmt, AVDictionary **options) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_open_input_, "avformat_open_input", kAvFormatModuleName);
  return avformat_open_input_(ps, url, fmt, options);
}

int AvcModuleProvider::avformat_write_header(AVFormatContext *s, AVDictionary **options) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avformat_write_header_, "avformat_write_header", kAvFormatModuleName);
  return avformat_write_header_(s, options);
}

AVIOContext *AvcModuleProvider::avio_alloc_context(
  unsigned char *buffer, int buffer_size, int write_flag, void *opaque,
  int (*read_packet)(void *opaque, uint8_t *buf, int buf_size),
  int (*write_packet)(void *opaque, uint8_t *buf, int buf_size),
  int64_t (*seek)(void *opaque, int64_t offset, int whence)) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avio_alloc_context_, "avio_alloc_context", kAvFormatModuleName);
  return avio_alloc_context_(buffer, buffer_size, write_flag, opaque, read_packet,
                             write_packet, seek);
}

void AvcModuleProvider::avio_context_free(AVIOContext** s) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avio_context_free_, "avio_context_free", kAvFormatModuleName);
  return avio_context_free_(s);
}

int AvcModuleProvider::avio_close(AVIOContext *s) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avio_close_, "avio_close", kAvFormatModuleName);
  return avio_close_(s);
}

int AvcModuleProvider::avio_closep(AVIOContext **s) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avio_closep_, "avio_closep", kAvFormatModuleName);
  return avio_closep_(s);
}

void AvcModuleProvider::avio_flush(AVIOContext *s) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avio_flush_, "avio_flush", kAvFormatModuleName);
  avio_flush_(s);
}

int AvcModuleProvider::avio_open2(AVIOContext **s, const char *url, int flags,
                                  const AVIOInterruptCB *int_cb, AVDictionary **options) {
  if (!avformat_handle_) Load();
  AVC_CHECK_AND_CALL(avio_open2_, "avio_open2", kAvFormatModuleName);
  return avio_open2_(s, url, flags, int_cb, options);
}

// avutils

unsigned AvcModuleProvider::avutil_version() {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(avutil_version_, "avutil_version", kAvUtilModuleName);
  return avutil_version_();
}

char *AvcModuleProvider::av_strdup(const char *s) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_strdup_, "av_strdup", kAvUtilModuleName);
  return av_strdup_(s);
}

int AvcModuleProvider::av_samples_get_buffer_size(int *linesize, int nb_channels,
                                                  int nb_samples,
                                                  int /*enum AVSampleFormat*/ sample_fmt,
                                                  int align) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_samples_get_buffer_size_, "av_samples_get_buffer_size", kAvUtilModuleName);
  return av_samples_get_buffer_size_(linesize, nb_channels, nb_samples, sample_fmt,
                                     align);
}

int64_t AvcModuleProvider::av_rescale_rnd(int64_t a, int64_t b, int64_t c,
                                          int /*enum AVRounding*/ rnd) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_rescale_rnd_, "av_rescale_rnd", kAvUtilModuleName);
  return av_rescale_rnd_(a, b, c, rnd);
}

int64_t AvcModuleProvider::av_rescale_q_rnd(int64_t a, AVRational bq, AVRational cq,
  int /*enum AVRounding*/ rnd) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_rescale_q_rnd_, "av_rescale_q_rnd", kAvUtilModuleName);
  return av_rescale_q_rnd_(a, bq, cq, rnd);
}


int AvcModuleProvider::av_samples_alloc(uint8_t **audio_data, int *linesize,
                                        int nb_channels, int nb_samples,
                                        int /*enum AVSampleFormat*/ sample_fmt,
                                        int /*(0 = default, 1 = no alignment)*/ align) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_samples_alloc_, "av_samples_alloc", kAvUtilModuleName);
  return av_samples_alloc_(audio_data, linesize, nb_channels, nb_samples, sample_fmt,
                           align);
}

int AvcModuleProvider::av_samples_alloc_array_and_samples(
  uint8_t ***audio_data, int *linesize, int nb_channels, int nb_samples,
  int /*enum AVSampleFormat*/ sample_fmt, int /*(0 = default, 1 = no alignment)*/ align) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_samples_alloc_array_and_samples_, "av_samples_alloc_array_and_samples", kAvUtilModuleName);
  return av_samples_alloc_array_and_samples_(audio_data, linesize, nb_channels,
                                             nb_samples, sample_fmt, align);
}

int AvcModuleProvider::av_opt_set_int(void *obj, const char *name, int64_t val,
                                      int search_flags) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_opt_set_int_, "av_opt_set_int", kAvUtilModuleName);
  return av_opt_set_int_(obj, name, val, search_flags);
}

int AvcModuleProvider::av_opt_set_sample_fmt(void *obj, const char *name,
                                             int /*enum AVSampleFormat*/ fmt,
                                             int search_flags) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_opt_set_sample_fmt_, "av_opt_set_sample_fmt", kAvUtilModuleName);
  return av_opt_set_sample_fmt_(obj, name, fmt, search_flags);
}

AVBufferRef *AvcModuleProvider::av_hwdevice_ctx_alloc(int /*enum AVHWDeviceType*/ type) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwdevice_ctx_alloc_, "av_hwdevice_ctx_alloc", kAvUtilModuleName);
  return av_hwdevice_ctx_alloc_(type);
}

int AvcModuleProvider::av_hwdevice_ctx_init(AVBufferRef *ref) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwdevice_ctx_init_, "av_hwdevice_ctx_init", kAvUtilModuleName);
  return av_hwdevice_ctx_init_(ref);
}

AVBufferRef *AvcModuleProvider::av_hwframe_ctx_alloc(AVBufferRef *device_ctx) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwframe_ctx_alloc_, "av_hwframe_ctx_alloc", kAvUtilModuleName);
  return av_hwframe_ctx_alloc_(device_ctx);
}

int AvcModuleProvider::av_hwdevice_find_type_by_name(const char *name) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwdevice_find_type_by_name_, "av_hwdevice_find_type_by_name", kAvUtilModuleName);
  return av_hwdevice_find_type_by_name_(name);
}

const char *AvcModuleProvider::av_hwdevice_get_type_name(int type) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwdevice_get_type_name_, "av_hwdevice_get_type_name", kAvUtilModuleName);
  return av_hwdevice_get_type_name_(type);
}

int /*enum AVHWDeviceType*/ AvcModuleProvider::av_hwdevice_iterate_types(
  int /* enum AVHWDeviceType*/ prev) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwdevice_iterate_types_, "av_hwdevice_iterate_types", kAvUtilModuleName);
  return av_hwdevice_iterate_types_(prev);
}

int AvcModuleProvider::av_hwdevice_ctx_create(AVBufferRef **device_ctx, int type,
                                              const char *device, AVDictionary *opts,
                                              int flags) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwdevice_ctx_create_, "av_hwdevice_ctx_create", kAvUtilModuleName);
  return av_hwdevice_ctx_create_(device_ctx, type, device, opts, flags);
}

int AvcModuleProvider::av_hwframe_ctx_init(AVBufferRef *ref) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwframe_ctx_init_, "av_hwframe_ctx_init", kAvUtilModuleName);
  return av_hwframe_ctx_init_(ref);
}

int AvcModuleProvider::av_hwframe_get_buffer(AVBufferRef *hwframe_ctx, AVFrame *frame,
                                             int flags) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwframe_get_buffer_, "av_hwframe_get_buffer", kAvUtilModuleName);
  return av_hwframe_get_buffer_(hwframe_ctx, frame, flags);
}

int AvcModuleProvider::av_hwframe_transfer_data(AVFrame *dst, const AVFrame *src,
                                                int flags) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwframe_transfer_data_, "av_hwframe_transfer_data", kAvUtilModuleName);
  return av_hwframe_transfer_data_(dst, src, flags);
}

int AvcModuleProvider::av_hwframe_transfer_get_formats(AVBufferRef *hwframe_ctx, int dir,
                                                       int **formats, int flags) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwframe_transfer_get_formats_, "av_hwframe_transfer_get_formats", kAvUtilModuleName);
  return av_hwframe_transfer_get_formats_(hwframe_ctx, dir, formats, flags);
}

void *AvcModuleProvider::av_hwdevice_hwconfig_alloc(AVBufferRef *device_ctx) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwdevice_hwconfig_alloc_, "av_hwdevice_hwconfig_alloc", kAvUtilModuleName);
  return av_hwdevice_hwconfig_alloc_(device_ctx);
}

AVHWFramesConstraints *AvcModuleProvider::av_hwdevice_get_hwframe_constraints(
  AVBufferRef *ref, const void *hwconfig) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwdevice_get_hwframe_constraints_, "av_hwdevice_get_hwframe_constraints", kAvUtilModuleName);
  return av_hwdevice_get_hwframe_constraints_(ref, hwconfig);
}

void AvcModuleProvider::av_hwframe_constraints_free(AVHWFramesConstraints **constraints) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwframe_constraints_free_, "av_hwframe_constraints_free", kAvUtilModuleName);
  av_hwframe_constraints_free_(constraints);
}

int AvcModuleProvider::av_hwframe_map(AVFrame *dst, const AVFrame *src, int flags) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_hwframe_map_, "av_hwframe_map", kAvUtilModuleName);
  return av_hwframe_map_(dst, src, flags);
}

int AvcModuleProvider::av_dict_set(AVDictionary **pm, const char *key, const char *value,
                                   int flags) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_dict_set_, "av_dict_set", kAvUtilModuleName);
  return av_dict_set_(pm, key, value, flags);
}

int AvcModuleProvider::av_dict_set_int(AVDictionary **pm, const char *key, int64_t value,
                                       int flags) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_dict_set_int_, "av_dict_set_int", kAvUtilModuleName);
  return av_dict_set_int_(pm, key, value, flags);
}

AVDictionaryEntry *AvcModuleProvider::av_dict_get(const AVDictionary *m, const char *key,
                                                  const AVDictionaryEntry *prev,
                                                  int flags) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_dict_get_, "av_dict_get", kAvUtilModuleName);
  return av_dict_get_(m, key, prev, flags);
}

int AvcModuleProvider::av_dict_count(const AVDictionary *m) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_dict_count_, "av_dict_count", kAvUtilModuleName);
  return av_dict_count_(m);
}

void AvcModuleProvider::av_dict_free(AVDictionary **m) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_dict_free_, "av_dict_free", kAvUtilModuleName);
  av_dict_free_(m);
}

AVFrame *AvcModuleProvider::av_frame_alloc(void) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_frame_alloc_, "av_frame_alloc", kAvUtilModuleName);
  return av_frame_alloc_();
}

void AvcModuleProvider::av_frame_free(AVFrame **frame) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_frame_free_, "av_frame_free", kAvUtilModuleName);
  av_frame_free_(frame);
}

int AvcModuleProvider::av_frame_ref(AVFrame* dst, const AVFrame* src) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_frame_ref_, "av_frame_ref", kAvUtilModuleName);
  return av_frame_ref_(dst, src);
}

int AvcModuleProvider::av_frame_replace(AVFrame* dst, const AVFrame* src) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_frame_replace_, "av_frame_replace", kAvUtilModuleName);
  return av_frame_replace_(dst, src);
}

AVFrame* AvcModuleProvider::av_frame_clone(const AVFrame* src) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_frame_clone_, "av_frame_clone", kAvUtilModuleName);
  return av_frame_clone_(src);
}

void AvcModuleProvider::av_frame_unref(AVFrame* frame) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_frame_unref_, "av_frame_unref", kAvUtilModuleName);
  av_frame_unref_(frame);
}

void AvcModuleProvider::av_frame_move_ref(AVFrame* dst, AVFrame* src) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_frame_move_ref_, "av_frame_move_ref", kAvUtilModuleName);
  av_frame_move_ref_(dst, src);
}

int AvcModuleProvider::av_frame_get_buffer(AVFrame *frame, int align) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_frame_get_buffer_, "av_frame_get_buffer", kAvUtilModuleName);
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
  AVC_CHECK_AND_CALL(av_free_, "av_free", kAvUtilModuleName);
  av_free_(ptr);
}

void AvcModuleProvider::av_freep(void *ptr) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_freep_, "av_freep", kAvUtilModuleName);
  av_freep_(ptr);
}

int AvcModuleProvider::av_get_bytes_per_sample(int sample_fmt) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_get_bytes_per_sample_, "av_get_bytes_per_sample", kAvUtilModuleName);
  return av_get_bytes_per_sample_(sample_fmt);
}

int AvcModuleProvider::av_image_copy_to_buffer(uint8_t *dst, int dst_size,
                                               const uint8_t *const src_data[4],
                                               const int src_linesize[4], int pix_fmt,
                                               int width, int height, int align) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_image_copy_to_buffer_, "av_image_copy_to_buffer", kAvUtilModuleName);
  return av_image_copy_to_buffer_(dst, dst_size, src_data, src_linesize, pix_fmt, width,
                                  height, align);
}

int AvcModuleProvider::av_image_fill_arrays(uint8_t *dst_data[4], int dst_linesize[4],
                                            const uint8_t *src, int pix_fmt, int width,
                                            int height, int align) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_image_fill_arrays_, "av_image_fill_arrays", kAvUtilModuleName);
  return av_image_fill_arrays_(dst_data, dst_linesize, src, pix_fmt, width, height,
                               align);
}

int AvcModuleProvider::av_image_get_buffer_size(int pix_fmt, int width, int height,
                                                int align) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_image_get_buffer_size_, "av_image_get_buffer_size", kAvUtilModuleName);
  return av_image_get_buffer_size_(pix_fmt, width, height, align);
}

void AvcModuleProvider::av_log_default_callback(void *avcl, int level, const char *fmt,
                                                va_list vl) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_log_default_callback_, "av_log_default_callback", kAvUtilModuleName);
  return av_log_default_callback_(avcl, level, fmt, vl);
}

void AvcModuleProvider::av_log_set_callback(void (*callback)(void *, int, const char *,
                                                             va_list)) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_log_set_callback_, "av_log_set_callback", kAvUtilModuleName);
  return av_log_set_callback_(callback);
}

void AvcModuleProvider::av_log_set_level(int level) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_log_set_level_, "av_log_set_level", kAvUtilModuleName);
  return av_log_set_level_(level);
}

void *AvcModuleProvider::av_malloc(size_t size) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_malloc_, "av_malloc", kAvUtilModuleName);
  return av_malloc_(size);
}

AVBufferRef *AvcModuleProvider::av_buffer_create(uint8_t *data, int size,
                                                 void (*free)(void *opaque,
                                                              uint8_t *data),
                                                 void *opaque, int flags) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_buffer_create_, "av_buffer_create", kAvUtilModuleName);
  return av_buffer_create_(data, size, free, opaque, flags);
}

int AvcModuleProvider::av_buffer_is_writable(const AVBufferRef *buf) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_buffer_is_writable_, "av_buffer_is_writable", kAvUtilModuleName);
  return av_buffer_is_writable_(buf);
}

void *AvcModuleProvider::av_buffer_get_opaque(const AVBufferRef *buf) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_buffer_get_opaque_, "av_buffer_get_opaque", kAvUtilModuleName);
  return av_buffer_get_opaque_(buf);
}

int AvcModuleProvider::av_buffer_get_ref_count(const AVBufferRef *buf) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_buffer_get_ref_count_, "av_buffer_get_ref_count", kAvUtilModuleName);
  return av_buffer_get_ref_count_(buf);
}

int AvcModuleProvider::av_buffer_make_writable(AVBufferRef **buf) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_buffer_make_writable_, "av_buffer_make_writable", kAvUtilModuleName);
  return av_buffer_make_writable_(buf);
}

int AvcModuleProvider::av_buffer_realloc(AVBufferRef **pbuf, int size) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_buffer_realloc_, "av_buffer_realloc", kAvUtilModuleName);
  return av_buffer_realloc_(pbuf, size);
}

void AvcModuleProvider::av_buffer_unref(AVBufferRef **buf) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_buffer_unref_, "av_buffer_unref", kAvUtilModuleName);
  av_buffer_unref_(buf);
}

AVBufferRef *AvcModuleProvider::av_buffer_ref(AVBufferRef *buf) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_buffer_ref_, "av_buffer_ref", kAvUtilModuleName);
  return av_buffer_ref_(buf);
}

int AvcModuleProvider::av_pix_fmt_count_planes(int pix_fmt) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_pix_fmt_count_planes_, "av_pix_fmt_count_planes", kAvUtilModuleName);
  return av_pix_fmt_count_planes_(pix_fmt);
}

const AVPixFmtDescriptor* AvcModuleProvider::av_pix_fmt_desc_get(int /*enum AVPixelFormat*/ pix_fmt) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_pix_fmt_desc_get_, "av_pix_fmt_desc_get", kAvUtilModuleName);
  return av_pix_fmt_desc_get_(pix_fmt);
}

int AvcModuleProvider::av_sample_fmt_is_planar(int sample_fmt) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_sample_fmt_is_planar_, "av_sample_fmt_is_planar", kAvUtilModuleName);
  return av_sample_fmt_is_planar_(sample_fmt);
}

int AvcModuleProvider::av_samples_set_silence(uint8_t **audio_data, int offset,
                                              int nb_samples, int nb_channels,
                                              int sample_fmt) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_samples_set_silence_, "av_samples_set_silence", kAvUtilModuleName);
  return av_samples_set_silence_(audio_data, offset, nb_samples, nb_channels, sample_fmt);
}

int AvcModuleProvider::av_strerror(int errnum, char *errbuf, size_t errbuf_size) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_strerror_, "av_strerror", kAvUtilModuleName);
  return av_strerror_(errnum, errbuf, errbuf_size);
}

uint64_t AvcModuleProvider::av_get_channel_layout(const char *name) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_get_channel_layout_, "av_get_channel_layout", kAvUtilModuleName);
  return av_get_channel_layout_(name);
}

int AvcModuleProvider::av_get_channel_layout_nb_channels(uint64_t channel_layout) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_get_channel_layout_nb_channels_, "av_get_channel_layout_nb_channels", kAvUtilModuleName);
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
  AVC_CHECK_AND_CALL(av_get_channel_layout_channel_index_, "av_get_channel_layout_channel_index", kAvUtilModuleName);
  return av_get_channel_layout_channel_index_(channel_layout, channel);
}

uint64_t AvcModuleProvider::av_channel_layout_extract_channel(uint64_t channel_layout,
                                                              int index) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_channel_layout_extract_channel_, "av_channel_layout_extract_channel", kAvUtilModuleName);
  return av_channel_layout_extract_channel_(channel_layout, index);
}

const char *AvcModuleProvider::av_get_channel_name(uint64_t channel) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_get_channel_name_, "av_get_channel_name", kAvUtilModuleName);
  return av_get_channel_name_(channel);
}

const char *AvcModuleProvider::av_get_channel_description(uint64_t channel) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_get_channel_description_, "av_get_channel_description", kAvUtilModuleName);
  return av_get_channel_description_(channel);
}

int AvcModuleProvider::av_get_standard_channel_layout(unsigned index, uint64_t *layout,
                                                      const char **name) {
  if (!avutil_handle_) Load();
  AVC_CHECK_AND_CALL(av_get_standard_channel_layout_, "av_get_standard_channel_layout", kAvUtilModuleName);
  return av_get_standard_channel_layout_(index, layout, name);
}

// swscale
unsigned AvcModuleProvider::swscale_version() {
  if (!swscale_handle_) Load();
  AVC_CHECK_AND_CALL(swscale_version_, "swscale_version", kSwScaleModuleName);
  return swscale_version_();
}

void AvcModuleProvider::sws_freeContext(struct SwsContext *swsContext) {
  if (!swscale_handle_) Load();
  AVC_CHECK_AND_CALL(sws_freeContext_, "sws_freeContext", kSwScaleModuleName);
  return sws_freeContext_(swsContext);
}

struct SwsContext *AvcModuleProvider::sws_getContext(int srcW, int srcH, int srcFormat,
                                                     int dstW, int dstH, int dstFormat,
                                                     int flags, SwsFilter *srcFilter,
                                                     SwsFilter *dstFilter,
                                                     const double *param) {
  if (!swscale_handle_) Load();
  AVC_CHECK_AND_CALL(sws_getContext_, "sws_getContext", kSwScaleModuleName);
  return sws_getContext_(srcW, srcH, srcFormat, dstW, dstH,
                         dstFormat, flags, srcFilter,
                         dstFilter, param);
}

int AvcModuleProvider::sws_scale(struct SwsContext *c, const uint8_t *const srcSlice[],
                                 const int srcStride[], int srcSliceY, int srcSliceH,
                                 uint8_t *const dst[], const int dstStride[]) {
  if (!swscale_handle_) Load();
  AVC_CHECK_AND_CALL(sws_scale_, "sws_scale", kSwScaleModuleName);
  return sws_scale_(c, srcSlice, srcStride, srcSliceY, srcSliceH, dst, dstStride);
}

// swresample
unsigned AvcModuleProvider::swresample_version() {
  if (!swresample_handle_) Load();
  AVC_CHECK_AND_CALL(swresample_version_, "swresample_version", kSwResampleModuleName);
  return swresample_version_();
}

SwrContext *AvcModuleProvider::swr_alloc() {
  if (!swresample_handle_) Load();
  AVC_CHECK_AND_CALL(swr_alloc_, "swr_alloc", kSwResampleModuleName);
  return swr_alloc_();
}

int AvcModuleProvider::swr_init(SwrContext *s) {
  if (!swresample_handle_) Load();
  AVC_CHECK_AND_CALL(swr_init_, "swr_init", kSwResampleModuleName);
  return swr_init_(s);
}
int AvcModuleProvider::swr_is_initialized(SwrContext *s) {
  if (!swresample_handle_) Load();
  AVC_CHECK_AND_CALL(swr_is_initialized_, "swr_is_initialized", kSwResampleModuleName);
  return swr_is_initialized_(s);
}
void AvcModuleProvider::swr_free(SwrContext **s) {
  if (!swresample_handle_) Load();
  AVC_CHECK_AND_CALL(swr_free_, "swr_free", kSwResampleModuleName);
  swr_free_(s);
}
void AvcModuleProvider::swr_close(SwrContext *s) {
  if (!swresample_handle_) Load();
  AVC_CHECK_AND_CALL(swr_close_, "swr_close", kSwResampleModuleName);
  swr_close_(s);
}
int AvcModuleProvider::swr_convert(SwrContext *s, uint8_t **out, int out_count,
                                   const uint8_t **in, int in_count) {
  if (!swresample_handle_) Load();
  AVC_CHECK_AND_CALL(swr_convert_, "swr_convert", kSwResampleModuleName);
  return swr_convert_(s, out, out_count, in, in_count);
}

int64_t AvcModuleProvider::swr_get_delay(struct SwrContext *s, int64_t base) {
  if (!swresample_handle_) Load();
  AVC_CHECK_AND_CALL(swr_get_delay_, "swr_get_delay", kSwResampleModuleName);
  return swr_get_delay_(s, base);
}

// avdevice

unsigned AvcModuleProvider::avdevice_version() {
  if (!avdevice_handle_) Load();
  AVC_CHECK_AND_CALL(avdevice_version_, "avdevice_version", kAvDeviceModuleName);
  return avdevice_version_();
}

void AvcModuleProvider::avdevice_register_all() {
  if (!avdevice_handle_) Load();
  AVC_CHECK_AND_CALL(avdevice_register_all_, "avdevice_register_all", kAvDeviceModuleName);
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
