
#ifndef I_AVC_MODULE_DATA_WRAPPER_FACTORY
#define I_AVC_MODULE_DATA_WRAPPER_FACTORY

#include <avc/i_avc_module_provider.h>
#include <memory>

namespace avc {

struct IAvcModuleDataWrapperFactory {
  virtual ~IAvcModuleDataWrapperFactory() = default;

  virtual const AvcModuleVersion& GetVersion() const = 0;
  virtual std::shared_ptr<IAvcModuleDataWrapper> 
      Create(std::shared_ptr<IAvcModuleProvider> module_provider) = 0;
};

}//namespace avc

#endif //I_AVC_MODULE_DATA_WRAPPER_FACTORY
