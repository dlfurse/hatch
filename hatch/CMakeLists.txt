##########
# global #
##########

cmake_minimum_required(VERSION 3.18.2)
cmake_policy(SET CMP0048 NEW)
project(hatch
    VERSION 0.1.0
    LANGUAGES C CXX)

add_definitions(-Wall)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#########
# gtest #
#########

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

############
# libressl #
############

#configure_file(CMakeLists.libressl.in.txt libressl/download/CMakeLists.txt)
#execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#    RESULT_VARIABLE result
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/libressl/download)
#
#if(result)
#  message(FATAL_ERROR "CMake step for libressl failed: ${result}")
#endif()
#
#execute_process(COMMAND ${CMAKE_COMMAND} --build .
#    RESULT_VARIABLE result
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/libressl/download)
#
#if(result)
#  message(FATAL_ERROR "Build step for libressl failed: ${result}")
#endif()
#
#add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/libressl/src
#    ${CMAKE_CURRENT_BINARY_DIR}/libressl/build
#    EXCLUDE_FROM_ALL)

###########
# utility #
###########

set(hatch_utility_sources.hh
  hatch/utility/rewrapper.hh

  hatch/core/pointer_list.hh
  hatch/core/pointer_list_fwd.hh
  hatch/core/pointer_list_node.hh
  hatch/core/pointer_list_node_impl.hh
  hatch/core/pointer_list_iterator.hh
  hatch/core/pointer_list_iterator_impl.hh
  hatch/core/pointer_list_root.hh
  hatch/core/pointer_list_root_impl.hh

  hatch/core/indexed_list.hh
  hatch/core/indexed_list_fwd.hh
  hatch/core/indexed_list_node.hh
  hatch/core/indexed_list_node_impl.hh
  hatch/core/indexed_list_iterator.hh
  hatch/core/indexed_list_iterator_impl.hh

  hatch/core/pointer_tree.hh
  hatch/core/pointer_tree_impl.hh
)

########
# core #
########

set(hatch_core_sources
  hatch/core/memory.hh
  hatch/core/memory_fwd.hh
  hatch/core/pointer.hh
  hatch/core/pointer_impl.hh
  hatch/core/allocator.hh
  hatch/core/allocator_impl.hh

  hatch/core/async.hh
  hatch/core/async_fwd.hh
  hatch/core/promise.hh
  hatch/core/promise_impl.hh
  hatch/core/future.hh
  hatch/core/future_impl.hh

#  hatch/core/streamer.hh
#  hatch/core/streamer.cc
  hatch/core/socket.hh
  hatch/core/reactor.hh
  hatch/core/reactor.cc
)

add_library(hatch_core STATIC ${hatch_core_sources})
set_property(TARGET hatch_core PROPERTY CXX_STANDARD 17)

########
# test #
########

set(hatch_utility_test_sources
  test/utility/pointer_list.cc
  test/utility/indexed_list.cc
)

add_executable(hatch_utility_test ${hatch_utility_test_sources})
set_property(TARGET hatch_utility_test PROPERTY CXX_STANDARD 17)
target_link_libraries(hatch_utility_test gtest_main)

add_test(NAME hatch_utility_test COMMAND hatch_utility_test)

#set(hatch_core_test_sources
#  test/core/memory.cc
#  test/core/async.cc
#  test/core/buffer.cc
#  test/core/socket.cc
#)
#
#add_executable(hatch_core_test ${hatch_core_test_sources})
#set_property(TARGET hatch_core_test PROPERTY CXX_STANDARD 17)
#target_link_libraries(hatch_core_test gtest_main)
#
#add_test(NAME hatch_core_test COMMAND hatch_core_test)
