
#ifndef DYNAMIC_LOADER_HPP
#define DYNAMIC_LOADER_HPP

#include <memory>
#include <sstream>

#include <tools/i_dynamic_modules_loader.h>

#ifdef _WIN32
#include <Windows.h>
#define GET_PROC_ADDRESS GetProcAddress
#else  //_WIN32
#include <dlfcn.h>
#define GET_PROC_ADDRESS dlsym
#endif  //_WIN32

template<typename T=void*> 
T LoadModule(const char* name) {
  T module_handle = (T)
#ifdef _WIN32
    reinterpret_cast<void*>(LoadLibraryA(name));
#else   // UNIX
    dlopen(name, RTLD_LAZY);
#endif  // _WIN32
  return module_handle;
}

template<typename T=void*>
void UnloadModule(T handle) {
#ifdef _WIN32
  FreeLibrary(reinterpret_cast<HMODULE>(handle));
#else  // UNIX
  dlclose((void*)handle);
#endif /*_WIN32*/
}

template<typename T>
bool LoadDllProc(T& t, void* mod_handle, const char* name) {
  if (!mod_handle) {
    t = nullptr;
    return false;
  }

  void* p = (void*) GET_PROC_ADDRESS(
      #ifdef _WIN32
        (HMODULE)
      #endif /*_WIN32*/
        mod_handle, name);
  t = reinterpret_cast<T>(p);
  if (!p)
    return false;

  return true;
}

template<typename T>
bool LoadDllProc(T& t, void* mod_handle, const char* name, std::stringstream& not_loaded_fns) {
  bool ret = LoadDllProc(t, mod_handle, name);
  if (!ret) 
    not_loaded_fns << " " << name;

  return ret;
}


namespace cmf {
struct IDynamicModulesLoader;
}//namespace cmf

template<typename T>
bool LoadDllProc(T& t, const std::shared_ptr<cmf::IDynamicModulesLoader>& mod_loader, void* mod_handle, const char* name) {
  void* p = mod_loader->GetProcAddress(mod_handle, name);
  t = reinterpret_cast<T>(p);
  if (!p)
    return false;

  return true;
}

template<typename T>
bool LoadDllProc(T& t, const std::shared_ptr<cmf::IDynamicModulesLoader>& mod_loader, void* mod_handle, const char* name, std::stringstream& not_loaded_fns) {
  void* p = mod_loader->GetProcAddress(mod_handle, name);
  t = reinterpret_cast<T>(p);
  if (!p) {
    not_loaded_fns << " " << name;
    return false;
  }

  return true;
}

class DynamicLoaderHelper {
public:
  DynamicLoaderHelper(std::shared_ptr<cmf::IDynamicModulesLoader> modules_loader, void* module_handle = nullptr)
    :modules_loader_(modules_loader)
    ,module_handle_(module_handle)
    ,has_errors_(false) {
  }

  DynamicLoaderHelper(void* module_handle = nullptr)
    :module_handle_(module_handle)
    ,has_errors_(false) {
  }

  DynamicLoaderHelper& SetModuleHandle(void* module_handle) {
    module_handle_ = module_handle;
    return *this;
  }

  template<typename T>
  DynamicLoaderHelper& LoadProc(const char* name, T& to_variable) {
    if (modules_loader_) {
      if (!LoadDllProc(to_variable, modules_loader_, module_handle_, name, not_loaded_fns_))
        has_errors_ = true;
    }
    return *this;
  }

  const std::stringstream& ProblemFunctions() const {
    return not_loaded_fns_;  
  }

  bool HasErrors() const {
    return has_errors_;
  }

private:
  std::stringstream not_loaded_fns_;
  std::shared_ptr<cmf::IDynamicModulesLoader> modules_loader_;
  void* module_handle_;
  bool has_errors_;
};

#endif //DYNAMIC_LOADER_HPP
