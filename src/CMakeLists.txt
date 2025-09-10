project(ffmpeg-loader LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "v")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #Optional

file(GLOB_RECURSE PUBLIC_HEADERS "${FFMPEGLOADER_PROJECT_ROOT_DIR}/include/*.h" "${FFMPEGLOADER_PROJECT_ROOT_DIR}/include/*.hpp")
file(GLOB_RECURSE EXTERNAL_SOURCES "${FFMPEGLOADER_PROJECT_ROOT_DIR}/external/*.cc")
file(GLOB PRIVATE_SOURCES *.c *.cc *.h *.hpp *.def)

if(FFMPEGLOADER_LOAD_AVC_STATICALLY)  
  set(STATIC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/static/ffmpeg-static-loader.cc")
else() #FFMPEGLOADER_LOAD_AVC_STATICALLY
  set(STATIC_SOURCES "")    
endif() #FFMPEGLOADER_LOAD_AVC_STATICALLY

if(FFMPEGLOADER_BUILD_SHARED)
  # Building DLL
  add_library(ffmpeg-loader SHARED
    ${PUBLIC_HEADERS}
    ${PRIVATE_SOURCES}
    ${STATIC_SOURCES}
  )

  target_compile_definitions(ffmpeg-loader PUBLIC FFMPEG_LOADER_DLL=1 DEBUG_PRINT=${FFMPEGLOADER_DEBUG_PRINT})
  target_sources(ffmpeg-loader PRIVATE ${EXTERNAL_SOURCES})

  if(WIN32)
    set_target_properties(ffmpeg-loader PROPERTIES
      WINDOWS_EXPORT_ALL_SYMBOLS OFF
      LINK_FLAGS "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg-loader.def"
    )
  endif()
else() # FFMPEGLOADER_BUILD_SHARED
  # Building static library
  add_library(ffmpeg-loader STATIC
    ${PUBLIC_HEADERS}
    ${PRIVATE_SOURCES}
    ${EXTERNAL_SOURCES}
    ${STATIC_SOURCES}
  )

  target_compile_definitions(ffmpeg-loader PUBLIC FFMPEG_LOADER_DLL=0 DEBUG_PRINT=${FFMPEGLOADER_DEBUG_PRINT})
  target_sources(ffmpeg-loader PRIVATE ${EXTERNAL_SOURCES})

  set_target_properties(ffmpeg-loader PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif() # FFMPEGLOADER_BUILD_SHARED

target_include_directories(ffmpeg-loader PUBLIC
  ${FFMPEGLOADER_PROJECT_ROOT_DIR}/include
)

target_include_directories(ffmpeg-loader PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# When FFmpeg libraries are planned to load statically, package includes and libs are necessary
if(FFMPEGLOADER_LOAD_AVC_STATICALLY)  
  target_compile_definitions(ffmpeg-loader PUBLIC AVC_LIBRARIES_STATIC_LINK=1)

  if("${FFMPEGLOADER_FFMPEG_INCLUDE_DIR}" STREQUAL "") # User was not set FFMPEGLOADER_FFMPEG_INCLUDE_DIR - try to find package via find_package
    # VCPkg support
    if(DEFINED ENV{VCPKG_ROOT})
      set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
    endif()
  
    find_package(FFmpeg REQUIRED)  
    if(FFmpeg_FOUND)
      target_link_libraries(ffmpeg-loader PRIVATE
        FFmpeg::avcodec
        FFmpeg::avdevice
        FFmpeg::avformat
        FFmpeg::avutil
        FFmpeg::swscale
        FFmpeg::swresample
      )
    endif()
  else()  # User manually provided FFMPEGLOADER_FFMPEG_INCLUDE_DIR / FFMPEGLOADER_FFMPEG_LIB_DIR
    target_include_directories(ffmpeg-loader PRIVATE ${FFMPEGLOADER_FFMPEG_INCLUDE_DIR})
    target_link_directories(ffmpeg-loader PRIVATE ${FFMPEGLOADER_FFMPEG_LIB_DIR})
    target_link_libraries(ffmpeg-loader PRIVATE avcodec avdevice avutil avfilter avformat swresample swscale)
  endif()   
endif() # FFMPEGLOADER_LOAD_AVC_STATICALLY
