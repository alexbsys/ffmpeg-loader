

# This file provides function that parses ffmpeg-versions.txt and process FFmpeg versions.
# For each FFmpeg version:
#   1. All headers libraries are downloaded to (ROOT)/external/ffmpeg-(version)
#   2. Automatically created loader file in (ROOT)/external/ffmpeg-(version)/ffmpeg-(version)-loader.cc
#   3. Fill (ROOT)/external/ffmpeg-versions.h and (ROOT)/external/ffmpeg-versions-register.cc
#
# Example content of ffmpeg-versions.txt
#    3.2 : release/3.2
#    3.4 : release/3.4  
#    8.1 : master
#    4.4 : release/4.4
#    5.1 : release/5.1

function(process_ffmpeg_versions)
    # Define parameters
    set(oneValueArgs VERSIONS_FILE)
    set(multiValueArgs)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    if(NOT ARG_VERSIONS_FILE)
        message(FATAL_ERROR "VERSIONS_FILE parameter is required")
    endif()
    
    if(NOT EXISTS "${ARG_VERSIONS_FILE}")
        message(FATAL_ERROR "Versions file not found: ${ARG_VERSIONS_FILE}")
    endif()
    
	# Recreate ffmpeg-versions.h 
    file(WRITE "${FFMPEGLOADER_EXTERNAL_BASE_DIR}/ffmpeg-versions.h" 
	"// THIS FILE IS CREATED AUTOMATICALLY BY CMAKE SCRIPTS. DO NOT EDIT
#ifndef FFMPEG_VERSIONS_HEADER
#define FFMPEG_VERSIONS_HEADER

#include <memory>
#include <vector>
#include <avc/i_avc_module_provider.h>
#include <i_avc_module_data_wrapper_factory.h>

extern std::vector<std::shared_ptr<avc::IAvcModuleDataWrapperFactory> > g_ffmpeg_data_wrappers;
")

    # Recreate ffmpeg-versions-register.cc
    file(WRITE "${FFMPEGLOADER_EXTERNAL_BASE_DIR}/ffmpeg-versions-register.cc" 
"// THIS FILE IS CREATED AUTOMATICALLY BY CMAKE SCRIPTS. DO NOT EDIT
#include \"ffmpeg-versions.h\"

std::shared_ptr<avc::IAvcModuleDataWrapperFactory> AvcModuleProviderDataWrapFactory_Static_Create();

void AvcDataProvidersGlobalInit() {
  if (g_ffmpeg_data_wrappers.size() != 0)
    return;

#if !defined(AVC_LIBRARIES_STATIC_LINK) || AVC_LIBRARIES_STATIC_LINK==0		
"	)


    # Start version file processing
    # Read the entire file
    file(READ "${ARG_VERSIONS_FILE}" file_content)
    
    # Split into lines
    string(REPLACE "\n" ";" lines_list "${file_content}")
    
    # Process each line
    foreach(line IN LISTS lines_list)
        # Remove leading/trailing whitespace
        string(STRIP "${line}" stripped_line)
        
        # Skip empty lines and comments
        if("${stripped_line}" STREQUAL "" OR "${stripped_line}" MATCHES "^#")
            continue()
        endif()
        
        # Split line by colon
        string(REPLACE ":" ";" line_parts "${stripped_line}")
        list(LENGTH line_parts parts_count)
        
        if(parts_count LESS 2)
            message(WARNING "Skipping invalid line: ${stripped_line}")
            continue()
        endif()
        
        # Extract version and tag
        list(GET line_parts 0 raw_version)
        list(GET line_parts 1 raw_tag)
        
        # Remove whitespace
        string(STRIP "${raw_version}" FFMPEG_VERSION)
        string(STRIP "${raw_tag}" FFMPEG_TAG)
        
        # Skip if version or tag is empty
        if("${FFMPEG_VERSION}" STREQUAL "" OR "${FFMPEG_TAG}" STREQUAL "")
            message(WARNING "Skipping line with empty version or tag: ${stripped_line}")
            continue()
        endif()
        
        message(STATUS "Processing FFmpeg version: ${FFMPEG_VERSION}, tag: ${FFMPEG_TAG}")
        
        # Here you can call your processing function
        process_ffmpeg_version("${FFMPEG_VERSION}" "${FFMPEG_TAG}")
        
    endforeach()

    # Finalize ffmpeg-versions.h	
    file(APPEND "${FFMPEGLOADER_EXTERNAL_BASE_DIR}/ffmpeg-versions.h" "
#endif //FFMPEG_VERSIONS_HEADER
")
 
    # Finalize ffmpeg-versions-register.cc
    file(APPEND "${FFMPEGLOADER_EXTERNAL_BASE_DIR}/ffmpeg-versions-register.cc" 
"#else // !defined(AVC_LIBRARIES_STATIC_LINK) || AVC_LIBRARIES_STATIC_LINK==0
  g_ffmpeg_data_wrappers.push_back(AvcModuleProviderDataWrapFactory_Static_Create());
#endif //AVC_LIBRARIES_STATIC_LINK
}
"    )
endfunction()


# Function to process each FFmpeg version
# This function downloads headers files from specified FFmpeg version
function(process_ffmpeg_version version tag)
  message(STATUS "=== Download headers from FFmpeg ${version} ===")
  string(REPLACE "." "_" FFMPEG_CUR_VERSION_UNDERSCORE ${version})   # 3.2 -> 3_2
    
  if(NOT EXISTS "${FFMPEGLOADER_EXTERNAL_BASE_DIR}/ffmpeg-${version}")
    fetch_git_partial(
        GIT_URL "https://github.com/FFmpeg/FFmpeg.git"
        TAG "${tag}"
        SRC_PATHS
          "libavcodec"
          "libavdevice"
          "libavfilter"
          "libavformat"
          "libavutil"
          "libswresample"
          "libswscale"
  
        DEST_DIR "${FFMPEGLOADER_EXTERNAL_BASE_DIR}/ffmpeg-${version}"
        TEMP_DIR "${FFMPEGLOADER_PROJECT_ROOT_DIR}/tmp"
        FILE_PATTERNS "*.h"
      )

    fix_includes_in_file("${FFMPEGLOADER_EXTERNAL_BASE_DIR}/ffmpeg-${version}/libswresample/swresample.h")

    # Create libavutil/avconfig.h for each FFmpeg version
    file(WRITE "${FFMPEGLOADER_EXTERNAL_BASE_DIR}/ffmpeg-${version}/libavutil/avconfig.h" "
#ifndef AVUTIL_AVCONFIG_H
#define AVUTIL_AVCONFIG_H
#define AV_HAVE_BIGENDIAN 0
#define AV_HAVE_FAST_UNALIGNED 1
#endif /*AVUTIL_AVCONFIG_H*/
")
  
    # Create loader source file
    file(WRITE "${FFMPEGLOADER_EXTERNAL_BASE_DIR}/ffmpeg-${version}/ffmpeg-${FFMPEG_CUR_VERSION_UNDERSCORE}-loader.cc" 
"
#define AVC_MODULE_DATA_WRAPPER_NAMESPACE ffmpeg_${FFMPEG_CUR_VERSION_UNDERSCORE}
#define AVC_MODULE_DATA_WRAPPER_CLASSNAME AvcModuleProviderDataWrap_${FFMPEG_CUR_VERSION_UNDERSCORE}
#define AVC_MODULE_DATA_WRAPPER_FACTORYNAME AvcModuleProviderDataWrapFactory_${FFMPEG_CUR_VERSION_UNDERSCORE}

#include \"loader_common.h\"

extern \"C\" {
namespace AVC_MODULE_DATA_WRAPPER_NAMESPACE {
#include \"libavutil/avutil.h\"
#include \"libavdevice/avdevice.h\"
#include \"libavcodec/avcodec.h\"
#include \"libavformat/avformat.h\"
#include \"libavutil/imgutils.h\"
#include \"libswscale/swscale.h\"
#include \"libavutil/hwcontext.h\"

struct SwrContext;
#include \"libswresample/swresample.h\"
}// namespace
}// extern \"C\"

#include \"avc_module_data_wrapper.hpp\"
")

  endif()  # EXISTS "${FFMPEGLOADER_EXTERNAL_BASE_DIR}/ffmpeg-${version}"

  # Declare this version factory create function in  ffmpeg-versions.h
  file(APPEND "${FFMPEGLOADER_EXTERNAL_BASE_DIR}/ffmpeg-versions.h" "std::shared_ptr<avc::IAvcModuleDataWrapperFactory> AvcModuleProviderDataWrapFactory_${FFMPEG_CUR_VERSION_UNDERSCORE}_Create();
")
  # Add version registration procedure to ffmpeg-versions-register.cc
  file(APPEND "${FFMPEGLOADER_EXTERNAL_BASE_DIR}/ffmpeg-versions-register.cc" "  g_ffmpeg_data_wrappers.push_back(AvcModuleProviderDataWrapFactory_${FFMPEG_CUR_VERSION_UNDERSCORE}_Create());
")

  message(STATUS "Finished processing FFmpeg ${version}")
endfunction()


# fix swresample include header: replace #include "libswresample/version_major.h" to #include "version_major.h"
function(fix_includes_in_file file_path)
    if(NOT EXISTS "${file_path}")
        message(WARNING "File not found: ${file_path}")
        return()
    endif()
    
    # Читаем содержимое файла
    file(READ "${file_path}" file_content)
    
    # Заменяем include директивы
    string(REPLACE "#include \"libswresample/version_major.h\"" "#include \"version_major.h\"" new_content "${file_content}")
    string(REPLACE "#include \"libavcodec/version_major.h\"" "#include \"version_major.h\"" new_content "${new_content}")
    string(REPLACE "#include \"libavformat/version_major.h\"" "#include \"version_major.h\"" new_content "${new_content}")
    # Добавьте другие замены по необходимости
    
    # Если содержимое изменилось, записываем обратно
    if(NOT "${file_content}" STREQUAL "${new_content}")
        file(WRITE "${file_path}" "${new_content}")
        message(STATUS "Fixed includes in: ${file_path}")
    endif()
endfunction()
