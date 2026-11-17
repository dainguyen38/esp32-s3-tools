cmake_minimum_required(VERSION 3.16)

set(IDF_TARGET esp32s3)
set(COMPONENTS main)
set(PROJECT_VER "0.1.0")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32_s3_tools)

if(NOT IDF_VERSION_MAJOR EQUAL 5 OR
   NOT IDF_VERSION_MINOR EQUAL 5 OR
   NOT IDF_VERSION_PATCH EQUAL 4)
    message(FATAL_ERROR "esp32_s3_tools requires ESP-IDF v5.5.4")
endif()
