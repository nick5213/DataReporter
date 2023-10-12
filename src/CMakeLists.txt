project(dc_report)

cmake_minimum_required(VERSION 3.4.1)

#if (CMAKE_BUILD_TYPE MATCHES "Debug")
#option(MEMORY_CHECK         "Set to switch to build use SSE"  ON)
#add_definitions(-D MEMORY_CHECK)
#endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
add_compile_options(-v)
add_definitions(-D DC_REPORT_DEBUG)

get_filename_component(CMAKE_SOURCE_DIR
  "."
  ABSOLUTE
)

get_filename_component(CMAKE_CURRENT_SOURCE_DIR
  "."
  ABSOLUTE
)

MESSAGE(STATUS "src_dir = ${CMAKE_SOURCE_DIR}.")
MESSAGE(STATUS "current_src_dir = ${CMAKE_CURRENT_SOURCE_DIR}.")

if (ANDROID)
  set(ANDROID_SOURCE
    ${CMAKE_SOURCE_DIR}/call_interface/android/DataReporter.cpp
    ${CMAKE_SOURCE_DIR}/call_interface/android/JniOnLoad.cpp
    ${CMAKE_SOURCE_DIR}/platform/android/FileAndroid.cpp
    ${CMAKE_SOURCE_DIR}/platform/android/util/AndroidUtil.cpp
    ${CMAKE_SOURCE_DIR}/platform/android/util/JniEnvelope.cpp
    ${CMAKE_SOURCE_DIR}/platform/android/util/JniHelper.cpp
  )
endif ()

set(DATA_SOURCE
  ${CMAKE_SOURCE_DIR}/core/base/Buffer.cpp
  ${CMAKE_SOURCE_DIR}/core/base/File.cpp
  ${CMAKE_SOURCE_DIR}/core/base/MiniPBCoder.cpp
  ${CMAKE_SOURCE_DIR}/core/base/MmapedFile.cpp
  ${CMAKE_SOURCE_DIR}/core/base/PBUtility.cpp
  ${CMAKE_SOURCE_DIR}/core/base/RawInput.cpp
  ${CMAKE_SOURCE_DIR}/core/base/RawOutput.cpp
  ${CMAKE_SOURCE_DIR}/core/crypto/xxtea.c
  ${CMAKE_SOURCE_DIR}/core/reporter/DataProvider.cpp
  ${CMAKE_SOURCE_DIR}/core/reporter/FileInputStream.cpp
  ${CMAKE_SOURCE_DIR}/core/reporter/IoUtil.cpp
  ${CMAKE_SOURCE_DIR}/core/reporter/MemoryStream.cpp
  ${CMAKE_SOURCE_DIR}/core/reporter/Reporter.cpp
  ${CMAKE_SOURCE_DIR}/core/reporter/StringUtil.cpp
  ${CMAKE_SOURCE_DIR}/core/reporter/TimeUtil.cpp
  ${CMAKE_SOURCE_DIR}/core/thread/HandlerThread.cpp
)

set(DATA_HEADERS
  ${CMAKE_SOURCE_DIR}/core/base/Buffer.h
  ${CMAKE_SOURCE_DIR}/core/base/File.h
  ${CMAKE_SOURCE_DIR}/core/base/MiniPBCoder.h
  ${CMAKE_SOURCE_DIR}/core/base/MmapedFile.h
  ${CMAKE_SOURCE_DIR}/core/base/PBUtility.h
  ${CMAKE_SOURCE_DIR}/core/base/RawInput.h
  ${CMAKE_SOURCE_DIR}/core/base/RawOutput.h
  ${CMAKE_SOURCE_DIR}/core/crypto/xxtea.c
  ${CMAKE_SOURCE_DIR}/core/reporter/DataProvider.h
  ${CMAKE_SOURCE_DIR}/core/reporter/FileInputStream.h
  ${CMAKE_SOURCE_DIR}/core/reporter/IoUtil.h
  ${CMAKE_SOURCE_DIR}/core/reporter/MemoryStream.h
  ${CMAKE_SOURCE_DIR}/core/reporter/Reporter.h
  ${CMAKE_SOURCE_DIR}/core/reporter/StringUtil.h
  ${CMAKE_SOURCE_DIR}/core/reporter/TimeUtil.h
  ${CMAKE_SOURCE_DIR}/core/thread/HandlerThread.h
)

set(FLUTTER_SOURCE
  dc/dc_report.cc
  dart/xdart_channel.cc
  flutter/flutter_data_report.cc
)

add_definitions(-DHAVE_EXPAT_CONFIG_H)

set(SOURCE_FILES
  ${DATA_SOURCE}
  ${FLUTTER_SOURCE}
  ${ANDROID_SOURCE}
)

add_executable(data-reporter
  main.cc
  ${SOURCE_FILES}
)

target_link_libraries(
  data-reporter
  z
)

target_include_directories(data-reporter
  PUBLIC
  core/base
  core/crypto
  core/reporter
  core/thread
  .
)