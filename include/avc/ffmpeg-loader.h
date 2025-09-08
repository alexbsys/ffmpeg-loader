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
