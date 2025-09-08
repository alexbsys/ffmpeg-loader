
cmake_minimum_required(VERSION 3.14)

project(generate_video VERSION 0.0.1.1 LANGUAGES C CXX)

#set(CMAKE_CXX_STANDARD 14 CACHE STRING "v")
#set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #Optional
set(SOURCE_FILES
  generate_video_ffmpeg_loader.cc
)

add_executable(generate_video ${SOURCE_FILES} ${DELEGATES_LIB_HEADER_FILES})
target_include_directories(generate_video PRIVATE "${PROJECT_ROOT_DIR}/include")

target_link_libraries(generate_video PRIVATE ffmpeg-loader)
#install(TARGETS cpp-delegates-example DESTINATION ../out)
