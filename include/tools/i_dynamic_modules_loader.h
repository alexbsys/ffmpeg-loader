
#ifndef I_DYNAMIC_MODULES_LOADER_HEADER
#define I_DYNAMIC_MODULES_LOADER_HEADER

#include <string>

namespace cmf {

struct IDynamicModulesLoader {
  virtual ~IDynamicModulesLoader() {}

  virtual std::string GetCurrentExecutableDir() = 0;
  virtual void* LoadModule(const std::string& module_path) = 0;
  virtual void* GetProcAddress(void* module_handle, const std::string& function_name) = 0;
  virtual void UnloadModule(void* module_handle) = 0;

  template<typename T>
  bool LoadModuleProc(T& out_fn_ptr, void* mod_handle, const std::string& function_name) {
    void* p = GetProcAddress(mod_handle, function_name);
    out_fn_ptr = reinterpret_cast<T>(p);
    return !!p;
  }
};

}//namespace cmf

#endif //I_DYNAMIC_MODULES_LOADER_HEADER
