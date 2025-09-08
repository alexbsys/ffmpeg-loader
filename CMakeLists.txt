cmake_minimum_required(VERSION 3.14)
project(ffmpeg-loader)

option(FFMPEGLOADER_BUILD_SHARED "FFmpeg loader shared library build" ON)
option(FFMPEGLOADER_LOAD_AVC_STATICALLY "Load AVC statically (requires libavxxx installed)" OFF)
option(FFMPEGLOADER_BUILD_EXAMPLES "Build FFmpeg loader examples" ON)
set(FFMPEGLOADER_FFMPEG_INCLUDE_DIR "" CACHE STRING "FFmpeg include directory (make sense only when LOAD_AVC_STATICALLY=ON)")
set(FFMPEGLOADER_FFMPEG_LIB_DIR "" CACHE STRING "FFmpeg lib directory (make sense only when LOAD_AVC_STATICALLY=ON)")

set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR} CACHE INTERNAL "Project root directory")
set(EXTERNAL_CMAKE_DIR ${PROJECT_ROOT_DIR}/external/cmake CACHE INTERNAL "External cmake directory")
set(EXTERNAL_BASE_DIR ${PROJECT_ROOT_DIR}/external CACHE INTERNAL "External libraries directory")

# Include all .cmake from current directory
file(GLOB MODULE_CMAKE_FILES ./*.cmake)
foreach(CMAKE_FILE ${MODULE_CMAKE_FILES})
  include(${CMAKE_FILE})
endforeach()

# Setup output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Process all FFmpeg versions
process_ffmpeg_versions(
  VERSIONS_FILE "${CMAKE_SOURCE_DIR}/ffmpeg-versions.txt"
)

if(DEFINED FFMPEG_VERSIONS)
  message(STATUS "Processed FFmpeg versions: ${FFMPEG_VERSIONS}")
  list(LENGTH FFMPEG_VERSIONS VERSION_COUNT)
  message(STATUS "Total versions processed: ${VERSION_COUNT}")
endif()

add_subdirectory(src)

if(FFMPEGLOADER_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
