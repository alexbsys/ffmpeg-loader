
#ifndef FFMPEG_LOADER_HEADER
#define FFMPEG_LOADER_HEADER

#include "i_avc_module_provider.h"
#include "i_avc_module_load_handler.h"
#include <memory>
#include <string>

namespace cmf {
struct IDynamicModulesLoader;
}//namespace cmf

namespace avc {

std::shared_ptr<cmf::IDynamicModulesLoader> CreateAvcDynamicModulesLoader();

std::shared_ptr<IAvcModuleProvider> CreateAvcModuleProvider(
    std::shared_ptr<cmf::IDynamicModulesLoader> modules_loader,
    std::shared_ptr<avc::IAvcModuleLoadHandler> load_handler = nullptr,
    const std::string& modules_path = std::string(),
    bool auto_load = true);
	
std::shared_ptr<IAvcModuleProvider> CreateAvcModuleProvider2(
  std::shared_ptr<cmf::IDynamicModulesLoader> modules_loader,
  std::shared_ptr<avc::IAvcModuleLoadHandler> load_handler,
  const std::string &modules_path,
  const std::string &avcodec_module_name,
  const std::string &avformat_module_name,
  const std::string &avutil_module_name,
  const std::string &avdevice_module_name,
  const std::string &swscale_module_name,
  const std::string &swresample_module_name, 
  bool auto_load = true);
	
std::shared_ptr<IAvcModuleProvider> CreateAvcModuleProvider3(
  const std::string& modules_path = std::string(),
  bool auto_load = true,
  std::shared_ptr<avc::IAvcModuleLoadHandler> load_handler = nullptr);
  
std::shared_ptr<IAvcModuleProvider> CreateAvcModuleProvider4(  
  const std::string& modules_path,
  const std::string& avcodec_module_name,
  const std::string& avformat_module_name,
  const std::string& avutil_module_name,
  const std::string& avdevice_module_name,
  const std::string& swscale_module_name,
  const std::string& swresample_module_name,
  bool auto_load = true,
  std::shared_ptr<avc::IAvcModuleLoadHandler> load_handler = nullptr);
	
}//namespace avc

#endif //FFMPEG_LOADER_HEADER
