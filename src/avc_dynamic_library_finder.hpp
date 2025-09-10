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

#ifndef DYNAMIC_LIBRARY_FINDER_HEADER
#define DYNAMIC_LIBRARY_FINDER_HEADER


#ifndef DEBUG_PRINT
#define DEBUG_PRINT 0
#endif //DEBUG_PRINT

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>

#if DEBUG_PRINT
#include <cstdio>
#endif //DEBUG_PRINT

namespace avc {
namespace detail {

namespace fs = std::filesystem;

class AvcDynamicLibraryFinder {
public:

  static std::vector<std::string> FindLibraryFiles(const std::string& directory_path,
    const std::string& module_name) {
    std::vector<std::string> results;

#if DEBUG_PRINT
    printf("AVCLOADER: Search module '%s' in directory '%s'\n", module_name.c_str(), directory_path.c_str());
#endif //DEBUG_PRINT

    try {
      for (const auto& entry : fs::directory_iterator(directory_path)) {
        if (entry.is_regular_file()) {
          std::string filename = entry.path().filename().string();

          if (MatchesPattern(filename, module_name)) {
            results.push_back(filename);
#if DEBUG_PRINT
            printf("AVCLOADER: Found matched module '%s' in directory '%s'\n", filename.c_str(), directory_path.c_str());
#endif //DEBUG_PRINT
          }
        }
      }
    }
    catch (const fs::filesystem_error&) {
      // no such directory
#if DEBUG_PRINT
      printf("AVCLOADER: Cannot find path '%s'\n", directory_path.c_str());
#endif //DEBUG_PRINT
    }

#if DEBUG_PRINT
    printf("AVCLOADER: Found %d matched modules in directory '%s'\n", static_cast<int>(results.size()), directory_path.c_str());
#endif //DEBUG_PRINT
    return results;
  }

private:
  static bool MatchesPattern(const std::string& filename, const std::string& module_name) {
    // check Windows pattern: avcodec-58.dll
    if (filename.find(module_name + "-") == 0 &&
      filename.find(".dll") == filename.length() - 4) {
      return true;
    }

    // check macOS pattern: libavcodec.58.dylib
    std::string mac_prefix = "lib" + module_name + ".";
    if (filename.find(mac_prefix) == 0 &&
      filename.find(".dylib") == filename.length() - 6) {
      return true;
    }

    // check Linux pattern: libavcodec.so.58
    std::string linux_prefix = "lib" + module_name + ".so.";
    if (filename.find(linux_prefix) == 0) {
      return true;
    }

    return false;
  }
};

}//namespace detail
}//namespace avc

#endif //DYNAMIC_LIBRARY_FINDER_HEADER
