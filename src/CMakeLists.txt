project(ffmpeg-loader)

file(GLOB PUBLIC_HEADERS include/*.h)
file(GLOB PRIVATE_SOURCES *.c *.cc *.h *.hpp)

file(GLOB_RECURSE EXTERNAL_SOURCES 
    "${PROJECT_ROOT_DIR}/external/*.cc"
    "${PROJECT_ROOT_DIR}/external/*.h"
)

if(FFMPEGLOADER_LOAD_AVC_STATICALLY)  
  set(STATIC_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/static/ffmpeg-static-loader.cc")
else() #FFMPEGLOADER_LOAD_AVC_STATICALLY
  set(STATIC_SOURCES "")    
endif() #FFMPEGLOADER_LOAD_AVC_STATICALLY

if (FFMPEGLOADER_BUILD_SHARED)
  add_library(ffmpeg-loader SHARED
    ${PUBLIC_HEADERS}
    ${PRIVATE_SOURCES}
	${EXTERNAL_SOURCES}
	${STATIC_SOURCES}
  )

  set_target_properties(ffmpeg-loader PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
    LINK_FLAGS "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg-loader.def"
  )
else() # FFMPEGLOADER_BUILD_SHARED
  add_library(ffmpeg-loader STATIC
    ${PUBLIC_HEADERS}
    ${PRIVATE_SOURCES}
	${EXTERNAL_SOURCES}
	${STATIC_SOURCES}
  )
endif() # FFMPEGLOADER_BUILD_SHARED

target_include_directories(ffmpeg-loader PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(ffmpeg-loader PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
	${PROJECT_ROOT_DIR}/include
)

if(FFMPEGLOADER_LOAD_AVC_STATICALLY)  # FFmpeg libraries are planned to load statically: include and libs are necessary
  set(STATIC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/static/ffmpeg-loader-static.cc)
  target_compile_definitions(ffmpeg-loader PUBLIC AVC_LIBRARIES_STATIC_LINK=1)

  if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
  endif()
  
  if("${FFMPEGLOADER_FFMPEG_INCLUDE_DIR}" STREQUAL "") # User was not set FFMPEGLOADER_FFMPEG_INCLUDE_DIR - try to find package via find_package
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
  else()  # User set FFMPEGLOADER_FFMPEG_INCLUDE_DIR / FFMPEGLOADER_FFMPEG_LIB_DIR
	target_include_directories(ffmpeg-loader PRIVATE ${FFMPEGLOADER_FFMPEG_INCLUDE_DIR})
	target_link_directories(ffmpeg-loader PRIVATE ${FFMPEGLOADER_FFMPEG_LIB_DIR})
	target_link_libraries(ffmpeg-loader PRIVATE avcodec avdevice avutil avfilter avformat swresample swscale)
  endif()   
endif() # FFMPEGLOADER_LOAD_AVC_STATICALLY
