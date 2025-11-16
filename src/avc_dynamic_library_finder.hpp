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
#include <string>
#include <vector>
#include <algorithm>

#if DEBUG_PRINT
#include <cstdio>
#endif //DEBUG_PRINT

// Platform-specific includes for directory iteration (C++14 compatible)
#ifdef _WIN32
  #include <windows.h>
#else
  #include <dirent.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
#endif

namespace avc {
namespace detail {

class AvcDynamicLibraryFinder {
public:

  static std::vector<std::string> FindLibraryFiles(const std::string& directory_path,
    const std::string& module_name) {
    std::vector<std::string> results;

#if DEBUG_PRINT
    printf("AVCLOADER: Search module '%s' in directory '%s'\n", module_name.c_str(), directory_path.c_str());
#endif //DEBUG_PRINT

#ifdef _WIN32
    // Windows implementation using FindFirstFile/FindNextFile
    std::string search_path = directory_path;
    if (!search_path.empty() && search_path.back() != '\\' && search_path.back() != '/') {
      search_path += "\\";
    }
    search_path += "*";

    WIN32_FIND_DATAA find_data;
    HANDLE hFind = FindFirstFileA(search_path.c_str(), &find_data);

    if (hFind != INVALID_HANDLE_VALUE) {
      do {
        // Skip directories
        if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
          std::string filename = find_data.cFileName;
          
          if (MatchesPattern(filename, module_name)) {
            results.push_back(filename);
#if DEBUG_PRINT
            printf("AVCLOADER: Found matched module '%s' in directory '%s'\n", filename.c_str(), directory_path.c_str());
#endif //DEBUG_PRINT
          }
        }
      } while (FindNextFileA(hFind, &find_data) != 0);
      
      FindClose(hFind);
    }
#if DEBUG_PRINT
    else {
      printf("AVCLOADER: Cannot find path '%s'\n", directory_path.c_str());
    }
#endif //DEBUG_PRINT

#else
    // Unix/Linux/macOS implementation using opendir/readdir
    DIR* dir = opendir(directory_path.c_str());
    if (dir != nullptr) {
      struct dirent* entry;
      while ((entry = readdir(dir)) != nullptr) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
          continue;
        }

        // Check if it's a regular file
        std::string full_path = directory_path;
        if (!full_path.empty() && full_path.back() != '/') {
          full_path += "/";
        }
        full_path += entry->d_name;

        struct stat st;
        if (stat(full_path.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
          std::string filename = entry->d_name;
          
          if (MatchesPattern(filename, module_name)) {
            results.push_back(filename);
#if DEBUG_PRINT
            printf("AVCLOADER: Found matched module '%s' in directory '%s'\n", filename.c_str(), directory_path.c_str());
#endif //DEBUG_PRINT
          }
        }
      }
      closedir(dir);
    }
#if DEBUG_PRINT
    else {
      printf("AVCLOADER: Cannot find path '%s'\n", directory_path.c_str());
    }
#endif //DEBUG_PRINT
#endif

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
