##########
# global #
##########

cmake_minimum_required(VERSION 2.8)
project(hatch C CXX)

set(MODULE_VERSION_MAJOR 0)
set(MODULE_VERSION_MINOR 1)
set(MODULE_VERSION_PATCH 0)
set(MODULE_VERSION "${MODULE_VERSION_MAJOR}.${MODULE_VERSION_MINOR}.${MODULE_VERSION_PATCH}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")


add_definitions(-Wall)
add_definitions(-std=c++17)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

configure_file(CMakeLists.gtest.in.txt googletest/download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
		RESULT_VARIABLE result
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest/download)

if(result)
	message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
		RESULT_VARIABLE result
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest/download)

if(result)
	message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest/src
		${CMAKE_CURRENT_BINARY_DIR}/googletest/build
		EXCLUDE_FROM_ALL)

###########
# utility #
###########

set(hatch_utility_sources.hh
	hatch/utility/rewrapper.hh
)

########
# core #
########

set(hatch_core_sources
	hatch/core/promise.hh
	hatch/core/promise_impl.hh
	hatch/core/future.hh
  hatch/core/future_impl.hh
	hatch/core/asynchronous.hh
	hatch/core/socket.hh
	hatch/core/reactor.hh
	hatch/core/reactor.cc
)

add_library(hatch_core STATIC ${hatch_core_sources})

########
# test #
########

set(hatch_core_test_sources
	test/core/asynchronous.cc
	test/core/socket.cc
)

add_executable(hatch_core_test ${hatch_core_test_sources})
target_link_libraries(hatch_core_test gtest_main)

add_test(NAME hatch_core_test COMMAND hatch_core_test)