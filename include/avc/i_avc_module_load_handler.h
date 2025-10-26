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

#ifndef I_AVC_MODULE_LOAD_HANDLER_HEADER
#define I_AVC_MODULE_LOAD_HANDLER_HEADER

namespace avc {

struct IAvcModuleProvider;

struct IAvcModuleLoadHandler {
  virtual ~IAvcModuleLoadHandler() = default;
  
  virtual void OnModuleLoad(IAvcModuleProvider* module_provider, const char* module_name) {
    (void)module_provider;
    (void)module_name;
  }
  
  // module_name contains AVCODEC, AVFORMAT, AVDEVICE, AVUTIL, SWSCALE, SWRESAMPLE
  virtual void OnModuleLoadError(IAvcModuleProvider* module_provider, const char* module_name) {
    (void)module_provider;
    (void)module_name;	  
  }

  virtual void OnModuleFunctionsNotFound(IAvcModuleProvider* module_provider, const char* module_name, const char* functions_list) {
    (void)module_provider;
    (void)module_name;
    (void)functions_list;
  }

  virtual void OnBeforeUnload(IAvcModuleProvider* module_provider) {
    (void)module_provider;	  
  }

  virtual void OnLoadFinished(IAvcModuleProvider* module_provider) {
    (void)module_provider;
  }
};	
	
}//namespace avc

#endif //I_AVC_MODULE_LOAD_HANDLER_HEADER
