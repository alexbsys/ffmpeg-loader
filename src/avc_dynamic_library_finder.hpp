
#ifndef DYNAMIC_LIBRARY_FINDER_HEADER
#define DYNAMIC_LIBRARY_FINDER_HEADER

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>

namespace avc {
namespace detail {

namespace fs = std::filesystem;

class AvcDynamicLibraryFinder {
public:

  static std::vector<std::string> FindLibraryFiles(const std::string& directory_path,
    const std::string& module_name) {
    std::vector<std::string> results;

    try {
      for (const auto& entry : fs::directory_iterator(directory_path)) {
        if (entry.is_regular_file()) {
          std::string filename = entry.path().filename().string();

          if (MatchesPattern(filename, module_name)) {
            results.push_back(filename);
          }
        }
      }
    }
    catch (const fs::filesystem_error&) {
      // no such directory
    }

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
