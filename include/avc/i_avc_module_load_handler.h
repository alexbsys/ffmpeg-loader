
#ifndef I_AVC_MODULE_LOAD_HANDLER_HEADER
#define I_AVC_MODULE_LOAD_HANDLER_HEADER

namespace avc {

struct IAvcModuleProvider;

struct IAvcModuleLoadHandler {
  virtual ~IAvcModuleLoadHandler() = default;
  
  virtual void OnModuleLoad(std::shared_ptr<IAvcModuleProvider> module_provider, const char* module_name) {
    (void)module_provider;
	(void)module_name;
  }
  
  // module_name contains AVCODEC, AVFORMAT, AVDEVICE, AVUTIL, SWSCALE, SWRESAMPLE
  virtual void OnModuleLoadError(std::shared_ptr<IAvcModuleProvider> module_provider, const char* module_name) {
    (void)module_provider;
	(void)module_name;	  
  }
  
  virtual void OnBeforeUnload(std::shared_ptr<IAvcModuleProvider> module_provider) {
    (void)module_provider;	  
  }
};	
	
}//namespace avc

#endif //I_AVC_MODULE_LOAD_HANDLER_HEADER
