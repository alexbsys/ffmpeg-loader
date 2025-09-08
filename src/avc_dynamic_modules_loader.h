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
