cmake_minimum_required(VERSION 3.12)
project(Simple-Profiler)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "build")
set (CMAKE_CXX_STANDARD 11)

include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckFunctionExists)
set(SRC_PATH "src/main/cpp")
set(SRC_TEST "src/test/cpp")
set(BIN "output")
set(OUTPUT "sagent")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN})

if (DEFINED ENV{JDK_HOME})
    include_directories($ENV{JDK_HOME}/include)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        include_directories($ENV{JDK_HOME}/include/darwin)
    endif()
else()
    message(FATAL_ERROR " JDK_HOME not define.")
endif()

set(SRC_FILES
    ${SRC_PATH}/agent.cc
    ${SRC_PATH}/common.h
    ${SRC_PATH}/common.cc
    ${SRC_PATH}/vm.cc
    ${SRC_PATH}/vm.h
    ${SRC_PATH}/profiler.cc
    ${SRC_PATH}/profiler.h
    ${SRC_PATH}/controller.cc
    ${SRC_PATH}/controller.h
    ${SRC_PATH}/signal_proc.cc
    ${SRC_PATH}/signal_proc.h
    ${SRC_PATH}/circle_queue.cc
    ${SRC_PATH}/circle_queue.h
)

add_library(sagent SHARED ${SRC_FILES})