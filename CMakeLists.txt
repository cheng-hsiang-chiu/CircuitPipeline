# CMake setup
cmake_minimum_required (VERSION 3.12)
MESSAGE(STATUS "CMAKE_ROOT: " ${CMAKE_ROOT})

# Project name
project(CircuitPipeline)

# Turn on the verbose
set(CMAKE_VERBOSE_MAKEFILE ON)

# CXX target properties
## g++
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.3")
    message(FATAL_ERROR "\nOpenTimer requires G++ at least v7.3 to build.")
  endif()
## clang++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "6.0")
    message(FATAL_ERROR "\nOpenTimer requires clang++ at least v6.0")
  endif() 
else()
  message(FATAL_ERROR "\n\
OpenTimer currently supports the following compilers:\n\
- g++ v7.3 or above\n\
- clang++ v6.0 or above\n\
")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")
endif()

# Check if static build
if(CMAKE_EXE_LINKER_FLAGS)
  string(FIND ${CMAKE_EXE_LINKER_FLAGS} "-static" IS_STATIC)
  if(NOT ${IS_STATIC} EQUAL -1)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--whole-archive ${CMAKE_THREAD_LIBS_INIT} -Wl,--no-whole-archive")
  endif()
endif()

message(STATUS "CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL: ${CMAKE_CXX_FLAGS_MINSIZEREL}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: " ${CMAKE_EXE_LINKER_FLAGS})
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

# The version number
# set(OT_VERSION_MAJOR "2")
# set(OT_VERSION_MINOR "1")
# set(OT_VERSION_PATCH "0")
# set(OT_VERSION "${OT_VERSION_MAJOR}.${OT_VERSION_MINOR}.${OT_VERSION_PATCH}")
set(CP_HOME ${PROJECT_SOURCE_DIR})
set(CP_BENCHMARK_DIR ${PROJECT_SOURCE_DIR}/benchmark)
set(CP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
#set(OT_UNITTEST_DIR ${PROJECT_SOURCE_DIR}/unittest)
# set(OT_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE)
# message(STATUS "OT_VERSION: ${OT_VERSION}")
message(STATUS "CP_HOME: ${CP_HOME}")
message(STATUS "CP_SOURCE_DIR: ${CP_SOURCE_DIR}")
message(STATUS "CP_BENCHMARK_DIR: ${CP_BENCHMARK_DIR}")
# message(STATUS "OT_UNITTEST_DIR: ${OT_UNITTEST_DIR}")
# message(STATUS "OT_LICENSE: ${OT_LICENSE}")


include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/3rd-party/)

find_package(Threads REQUIRED)
#set(THREADS_PREFER_PTHREAD_FLAG ON)

list(APPEND CP_LINK_FLAGS "")
list(APPEND CP_LINK_FLAGS Threads::Threads)
#list(APPEND CP_LINK_FLAGS stdc++fs)
message(STATUS "CP_LINK_FLAGS: ${CP_LINK_FLAGS}")


###########################################################
# Executables
###########################################################
message(STATUS "Building executables ...")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# # shell
# set(OT_SHELL ${PROJECT_SOURCE_DIR}/bin/ot-shell)
# add_executable(ot-shell main/shell/shell.cpp)
# target_link_libraries(ot-shell ${OT_LINK_FLAGS})
# message(STATUS "OpenTimer shell: ${OT_SHELL}")

###########################################################
# Example
###########################################################


add_executable(circuit_report ${CP_SOURCE_DIR}/circuit_report.cpp ${CP_SOURCE_DIR}/graph.hpp)
add_executable(circuit_generator ${CP_SOURCE_DIR}/circuit_generator.cpp ${CP_SOURCE_DIR}/graph.hpp)
add_executable(circuit_taskflow ${CP_SOURCE_DIR}/circuit_taskflow.cpp ${CP_SOURCE_DIR}/graph.hpp)
add_executable(circuit_pipeline ${CP_SOURCE_DIR}/circuit_pipeline.cpp ${CP_SOURCE_DIR}/graph.hpp)

target_link_libraries(circuit_taskflow ${CP_LINK_FLAGS})
target_link_libraries(circuit_pipeline ${CP_LINK_FLAGS})

#include_directories(${PROJECT_SOURCE_DIR}/benchmark)
# # simple
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example/simple)
# add_executable(simple example/simple/simple.cpp)
# target_link_libraries(simple ${OT_LINK_FLAGS})
# 
# # incremental
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example/incremental)
# add_executable(incremental example/incremental/incremental.cpp)
# target_link_libraries(incremental ${OT_LINK_FLAGS})
# 
# # unit
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example/unit)
# add_executable(unit example/unit/unit.cpp)
# target_link_libraries(unit ${OT_LINK_FLAGS})
# 
# # optimizer
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example/optimizer)
# add_executable(optimizer example/optimizer/optimizer.cpp)
# target_link_libraries(optimizer ${OT_LINK_FLAGS})
# 
# # sizer
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example/sizer)
# add_executable(sizer example/sizer/sizer.cpp)
# target_link_libraries(sizer ${OT_LINK_FLAGS})

##aes_cipher
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/example/aes_cipher)
#add_executable(aes_cipher example/aes_cipher/aes_cipher.cpp)
#target_link_libraries(aes_cipher ${OT_LINK_FLAGS})

###########################################################
# Install
# cmake .. -DCMAKE_INSTALL_PREFIX=<prefix_path>
###########################################################

# hpp
#foreach(file ${OT_HPP})
#  get_filename_component( dir ${file} DIRECTORY )
#  install(FILES ${file} DESTINATION include/${dir} )
#endforeach()

# # include
# install(DIRECTORY ${PROJECT_SOURCE_DIR}/ot DESTINATION include)
# 
# # library
# install(TARGETS OpenTimer DESTINATION lib)
# 
# # executable
# install(TARGETS ot-shell DESTINATION bin)

###########################################################
# Test
###########################################################

# Test 
# enable_testing()

# Unittest
# message(STATUS "Building unit tests ...")
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/unittest)
# 
# add_executable(utility unittest/utility.cpp)
# target_link_libraries(utility ${OT_LINK_FLAGS})
# 
# add_executable(path unittest/path.cpp)
# target_link_libraries(path ${OT_LINK_FLAGS})
# 
# add_test(ut.utility ${OT_UNITTEST_DIR}/utility -d yes)
# add_test(ut.path ${OT_UNITTEST_DIR}/path -d yes)
# 
# # Integration test on tau15 benchmark (generated by IBM Einstimer)
# message(STATUS "Building TAU15 integration tests ...")
# set(OT_TAU15_CHECKER ${PROJECT_SOURCE_DIR}/inttest/tau15.py)
# add_test(it.tau15.c3_slack ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c3_slack)
# add_test(it.tau15.c3_path ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c3_path)
# add_test(it.tau15.c17 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c17)
# add_test(it.tau15.c17_slack ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c17_slack)
# add_test(it.tau15.c432 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c432)
# add_test(it.tau15.c499 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c499)
# add_test(it.tau15.c880 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c880)
# add_test(it.tau15.c1355 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c1355)
# add_test(it.tau15.c1908 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c1908)
# add_test(it.tau15.c2670 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c2670)
# add_test(it.tau15.c3540 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c3540)
# add_test(it.tau15.c5315 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c5315)
# add_test(it.tau15.c6288 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c6288)
# add_test(it.tau15.c7552 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c7552)
# add_test(it.tau15.c7552_slack ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} c7552_slack)
# add_test(it.tau15.simple ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} simple)
# add_test(it.tau15.s27 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} s27)
# add_test(it.tau15.s27_path ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} s27_path)
# add_test(it.tau15.s27_spef ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} s27_spef)
# add_test(it.tau15.s344 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} s344)
# add_test(it.tau15.s349 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} s349)
# add_test(it.tau15.s386 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} s386)
# add_test(it.tau15.s400 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} s400)
# add_test(it.tau15.s510 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} s510)
# add_test(it.tau15.s526 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} s526)
# add_test(it.tau15.s1196 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} s1196)
# add_test(it.tau15.s1494 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} s1494)
# add_test(it.tau15.tv80 ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} tv80)
# add_test(it.tau15.wb_dma ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} wb_dma)
# add_test(it.tau15.usb_phy_ispd ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} usb_phy_ispd)
# add_test(it.tau15.ac97_ctrl ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} ac97_ctrl)
# add_test(it.tau15.aes_core ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} aes_core)
# add_test(it.tau15.des_perf ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} des_perf)
# add_test(it.tau15.vga_lcd ${OT_TAU15_CHECKER} ${OT_TAU15} ${OT_BENCHMARK_DIR} vga_lcd)
# 
# # Integration test for shell
# message(STATUS "Building Shell integration tests ...")
# set(OT_SHELL_CHECKER ${PROJECT_SOURCE_DIR}/inttest/shell.py)
# add_test(it.shell.c3_slack ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c3_slack)
# add_test(it.shell.c17 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c17)
# add_test(it.shell.c17_slack ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c17_slack)
# add_test(it.shell.c432 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c432)
# add_test(it.shell.c499 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c499)
# add_test(it.shell.c880 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c880)
# add_test(it.shell.c1355 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c1355)
# add_test(it.shell.c1908 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c1908)
# add_test(it.shell.c2670 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c2670)
# add_test(it.shell.c3540 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c3540)
# add_test(it.shell.c5315 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c5315)
# add_test(it.shell.c6288 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c6288)
# add_test(it.shell.c7552 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c7552)
# add_test(it.shell.c7552_slack ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} c7552_slack)
# add_test(it.shell.simple ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} simple)
# add_test(it.shell.s27 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} s27)
# add_test(it.shell.s344 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} s344)
# add_test(it.shell.s349 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} s349)
# add_test(it.shell.s386 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} s386)
# add_test(it.shell.s400 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} s400)
# add_test(it.shell.s510 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} s510)
# add_test(it.shell.s526 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} s526)
# add_test(it.shell.s1196 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} s1196)
# add_test(it.shell.s1494 ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} s1494)
# add_test(it.shell.ac97_ctrl ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} ac97_ctrl)
# add_test(it.shell.aes_core ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} aes_core)
# add_test(it.shell.des_perf ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} des_perf)
# add_test(it.shell.vga_lcd ${OT_SHELL_CHECKER} ${OT_SHELL} ${OT_BENCHMARK_DIR} vga_lcd)




