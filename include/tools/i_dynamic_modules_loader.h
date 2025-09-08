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

#ifndef I_DYNAMIC_MODULES_LOADER_HEADER
#define I_DYNAMIC_MODULES_LOADER_HEADER

#include <string>

namespace cmf {

struct IDynamicModulesLoader {
  virtual ~IDynamicModulesLoader() = default;

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
