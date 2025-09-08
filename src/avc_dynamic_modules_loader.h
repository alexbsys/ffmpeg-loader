#ifndef AVC_DYNAMIC_MODULES_LOADER_H
#define AVC_DYNAMIC_MODULES_LOADER_H

#include <string>
#include <memory>
#include <tools/i_dynamic_modules_loader.h>

namespace avc {
namespace detail {

class AvcDynamicModulesLoader 
  : public virtual cmf::IDynamicModulesLoader{
public:
  AvcDynamicModulesLoader();
  virtual ~AvcDynamicModulesLoader() override = default;

  std::string GetCurrentExecutableDir() override;
  void* LoadModule(const std::string& module_path) override;
  void* GetProcAddress(void* module_handle, const std::string& function_name) override;
  void UnloadModule(void* module_handle) override;
};

}//namespace detail

std::shared_ptr<cmf::IDynamicModulesLoader> CreateAvcDynamicModulesLoader();

}//namespace avc

#endif // DYNAMIC_MODULES_LOADER_H
