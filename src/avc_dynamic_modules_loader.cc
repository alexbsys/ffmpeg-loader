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

#include "avc_dynamic_modules_loader.h"

#include <cstring>
#include <cstdlib>

#ifdef _WIN32
#	include <windows.h>
#else //_WIN32
#	include <dlfcn.h>
#	include <limits.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <climits>
#else // not apple
#include <unistd.h>
#endif //__APPLE__

#endif //_WIN32


#ifndef _WIN32
#define LOG_SELF_PROC_LINK "/proc/self/exe"

static std::string do_readlink(std::string const& path) {
  char buff[1024];
  memset(buff, 0, sizeof(buff));
  ssize_t len = ::readlink(path.c_str(), buff, sizeof(buff) - 1);
  if (len != -1) {
    buff[len] = '\0';
    return std::string(buff);
  }
  return std::string();
}
#endif  //_WIN32

static std::string get_path_separator() {
#ifdef _WIN32
  return std::string("\\");
#else /*_WIN32*/
  return std::string("/");
#endif /*_WIN32*/
}

static std::string get_process_file_path() {
#ifdef _WIN32
  char file_name[MAX_PATH];
  GetModuleFileNameA(NULL, file_name, sizeof(file_name));
  std::string path(file_name);
  size_t last_delimiter = path.find_last_of(get_path_separator());
  return path.substr(0, last_delimiter);
#else  // _WIN32

#ifndef __APPLE__
  std::string path = do_readlink(LOG_SELF_PROC_LINK);
#else //__APPLE__
  char file_path[PATH_MAX];
  ::uint32_t size = sizeof(file_path);
  _NSGetExecutablePath(file_path, &size);
  std::string path = file_path;

#endif //__APPLE__
  return path.substr(0, path.find_last_of('/'));
#endif  // _WIN32
}

namespace avc {

std::shared_ptr<cmf::IDynamicModulesLoader> CreateAvcDynamicModulesLoader() {
  return std::make_shared<detail::AvcDynamicModulesLoader>();
}

namespace detail {

AvcDynamicModulesLoader::AvcDynamicModulesLoader() {}

std::string AvcDynamicModulesLoader::GetCurrentExecutableDir() {
  return get_process_file_path();
}

void* AvcDynamicModulesLoader::LoadModule(const std::string& module_path) {
  void* module_handle = nullptr;

#ifdef _WIN32
  module_handle = LoadLibraryA(module_path.c_str());
#else //_WIN32
  module_handle = dlopen(module_path.c_str(), RTLD_LAZY);
#endif //_WIN32

  return module_handle;
}

void* AvcDynamicModulesLoader::GetProcAddress(void* module_handle, const std::string& function_name) {
  void* fn_addr = nullptr;
#ifdef _WIN32
  fn_addr = reinterpret_cast<void*>(
      ::GetProcAddress(reinterpret_cast<HMODULE>(module_handle), function_name.c_str()));
#else //_WIN32
  fn_addr = dlsym(module_handle, function_name.c_str());
#endif //_WIN32
  return fn_addr;
}

void AvcDynamicModulesLoader::UnloadModule(void* module_handle) {
#ifdef _WIN32
    FreeLibrary(reinterpret_cast<HMODULE>(module_handle));
#else //_WIN32
    dlclose(module_handle);
#endif //_WIN32
}

}//namespace detail
}//namespace avc
