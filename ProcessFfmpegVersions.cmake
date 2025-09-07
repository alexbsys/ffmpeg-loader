# Function to parse ffmpeg-versions.txt and process each version
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
    
	# Recreate previous ffmpeg-versions.h and ffmpeg-versions-register.cc
    file(WRITE "${EXTERNAL_BASE_DIR}/ffmpeg-versions.h" 
	"// THIS FILE IS CREATED AUTOMATICALLY BY CMAKE SCRIPTS. DO NOT EDIT
#ifndef FFMPEG_VERSIONS_HEADER
#define FFMPEG_VERSIONS_HEADER

#include <memory>
#include <vector>
#include <mproc/i_avc_module_provider.h>

extern std::vector<std::shared_ptr<cmf::IAvcModuleDataWrapperFactory> > g_ffmpeg_data_wrappers;
")
 
    file(WRITE "${EXTERNAL_BASE_DIR}/ffmpeg-versions-register.cc" 
"// THIS FILE IS CREATED AUTOMATICALLY BY CMAKE SCRIPTS. DO NOT EDIT
#include \"ffmpeg-versions.h\"

std::shared_ptr<cmf::IAvcModuleDataWrapperFactory> AvcModuleProviderDataWrapFactory_Static_Create();

void AvcDataProvidersGlobalInit() {
  if (g_ffmpeg_data_wrappers.size() != 0)
    return;

#if !defined(AVC_LIBRARIES_STATIC_LINK) || AVC_LIBRARIES_STATIC_LINK==0		
"	)

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
	
    file(APPEND "${EXTERNAL_BASE_DIR}/ffmpeg-versions.h" 
	"
#endif //FFMPEG_VERSIONS_HEADER
")
 
    file(APPEND "${EXTERNAL_BASE_DIR}/ffmpeg-versions-register.cc" 
"#else // !defined(AVC_LIBRARIES_STATIC_LINK) || AVC_LIBRARIES_STATIC_LINK==0		
  g_ffmpeg_data_wrappers.push_back(AvcModuleProviderDataWrapFactory_Static_Create());
#endif //AVC_LIBRARIES_STATIC_LINK
}
"	)	
endfunction()

# Function to process each FFmpeg version (example implementation)
function(process_ffmpeg_version version tag)
  message(STATUS "=== Processing FFmpeg ${version} ===")
  string(REPLACE "." "_" FFMPEG_CUR_VERSION_UNDERSCORE ${version})
    
  if(NOT EXISTS "${EXTERNAL_BASE_DIR}/ffmpeg-${version}")

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
		  "libpostproc"
  
        DEST_DIR "${EXTERNAL_BASE_DIR}/ffmpeg-${version}"
        TEMP_DIR "${PROJECT_ROOT_DIR}/tmp"
        FILE_PATTERNS "*.h"
      )

	# Create libavutil/avconfig.h
	file(WRITE "${EXTERNAL_BASE_DIR}/ffmpeg-${version}/libavutil/avconfig.h" "
#ifndef AVUTIL_AVCONFIG_H
#define AVUTIL_AVCONFIG_H
#define AV_HAVE_BIGENDIAN 0
#define AV_HAVE_FAST_UNALIGNED 1
#endif /*AVUTIL_AVCONFIG_H*/
")
	  
    # Create loader source file
    file(WRITE "${EXTERNAL_BASE_DIR}/ffmpeg-${version}/ffmpeg-${FFMPEG_CUR_VERSION_UNDERSCORE}-loader.cc" 
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

  endif()	
  file(APPEND "${EXTERNAL_BASE_DIR}/ffmpeg-versions.h" "std::shared_ptr<cmf::IAvcModuleDataWrapperFactory> AvcModuleProviderDataWrapFactory_${FFMPEG_CUR_VERSION_UNDERSCORE}_Create();
")
  file(APPEND "${EXTERNAL_BASE_DIR}/ffmpeg-versions-register.cc" "  g_ffmpeg_data_wrappers.push_back(AvcModuleProviderDataWrapFactory_${FFMPEG_CUR_VERSION_UNDERSCORE}_Create());
")

  message(STATUS "Finished processing FFmpeg ${version}")
endfunction()

# Alternative implementation with more robust parsing
function(process_ffmpeg_versions_advanced)
    set(oneValueArgs VERSIONS_FILE)
    cmake_parse_arguments(ARG "" "${oneValueArgs}" "" ${ARGN})
    
    if(NOT EXISTS "${ARG_VERSIONS_FILE}")
        return()
    endif()
    
    # Read file line by line for better memory handling
    file(STRINGS "${ARG_VERSIONS_FILE}" lines)
    
    foreach(line IN LISTS lines)
        # Skip comments and empty lines
        if(line MATCHES "^[ \t]*#")
            continue()
        endif()
        
        string(STRIP "${line}" stripped_line)
        if("${stripped_line}" STREQUAL "")
            continue()
        endif()
        
        # Use regex for more robust parsing
        if(line MATCHES "^[ \t]*([^:#]+)[ \t]*:[ \t]*([^#]+)")
            set(FFMPEG_VERSION ${CMAKE_MATCH_1})
            set(FFMPEG_TAG ${CMAKE_MATCH_2})
            
            # Remove any trailing comments
            string(REGEX REPLACE "[ \t]+#.*$" "" FFMPEG_VERSION "${FFMPEG_VERSION}")
            string(REGEX REPLACE "[ \t]+#.*$" "" FFMPEG_TAG "${FFMPEG_TAG}")
            
            # Final cleanup
            string(STRIP "${FFMPEG_VERSION}" FFMPEG_VERSION)
            string(STRIP "${FFMPEG_TAG}" FFMPEG_TAG)
            
            if(NOT "${FFMPEG_VERSION}" STREQUAL "" AND NOT "${FFMPEG_TAG}" STREQUAL "")
                message(STATUS "Found version: '${FFMPEG_VERSION}' with tag: '${FFMPEG_TAG}'")
                
                # Process this version
                process_ffmpeg_version("${FFMPEG_VERSION}" "${FFMPEG_TAG}")
            endif()
        else()
            message(WARNING "Skipping malformed line: ${line}")
        endif()
    endforeach()
endfunction()

# Utility function to create versions file if it doesn't exist
function(create_default_ffmpeg_versions)
    set(VERSIONS_FILE "${CMAKE_SOURCE_DIR}/ffmpeg-versions.txt")
    
    if(NOT EXISTS "${VERSIONS_FILE}")
        message(STATUS "Creating default ffmpeg-versions.txt")
        
        set(DEFAULT_CONTENT "# Format: VERSION : TAG_NAME
# Supported FFmpeg versions
3.2 : release/3.2
3.4 : release/3.4  
6.0 : master
4.4 : release/4.4
5.1 : release/5.1
")
        file(WRITE "${VERSIONS_FILE}" "${DEFAULT_CONTENT}")
    endif()
	
endfunction()

# Example usage in your CMakeLists.txt
