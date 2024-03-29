##########
# global #
##########

cmake_minimum_required(VERSION 3.16.3)
project(hatch
    VERSION 0.1.0
    LANGUAGES C CXX)

add_definitions(-Wall)
add_definitions(-fno-rtti)
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

set(hatch_utility_sources
  hatch/utility/meta.hh
  hatch/utility/meta_fwd.hh
  hatch/utility/meta_impl.hh
  hatch/utility/meta_defs.hh

  hatch/utility/integral.hh
  hatch/utility/integral_fwd.hh
  hatch/utility/integral_impl.hh
  hatch/utility/integral_defs.hh

  hatch/utility/container.hh
  hatch/utility/container_fwd.hh
  hatch/utility/container_decl.hh
  hatch/utility/container_impl.hh

  hatch/utility/pointed.hh
  hatch/utility/pointed_fwd.hh
  hatch/utility/pointed_decl.hh
  hatch/utility/pointed_impl.hh

  hatch/utility/indexed.hh
  hatch/utility/indexed_fwd.hh
  hatch/utility/indexed_decl.hh
  hatch/utility/indexed_impl.hh
  hatch/utility/indexed_defs.hh

  hatch/utility/chain.hh
  hatch/utility/chain_fwd.hh
  hatch/utility/chain_decl.hh
  hatch/utility/chain_impl.hh

  hatch/utility/branch.hh
  hatch/utility/branch_fwd.hh
  hatch/utility/branch_decl.hh
  hatch/utility/branch_impl.hh
#
#  hatch/utility/owning.hh
#  hatch/utility/owning_fwd.hh
#  hatch/utility/owner.hh
#  hatch/utility/owner_impl.hh
#  hatch/utility/owned.hh
#  hatch/utility/owned_impl.hh
#
#  hatch/utility/list_fwd.hh
#  hatch/utility/list.hh
#  hatch/utility/list_impl.hh
#  hatch/utility/list_node.hh
#  hatch/utility/list_node_impl.hh
#  hatch/utility/list_iterator.hh
#  hatch/utility/list_iterator_impl.hh
#
#  hatch/utility/tree_fwd.hh
#  hatch/utility/tree.hh
#  hatch/utility/tree_impl.hh
#  hatch/utility/tree_node.hh
#  hatch/utility/tree_node_impl.hh
#  hatch/utility/tree_iterator.hh
#  hatch/utility/tree_iterator_impl.hh
)

########
# core #
########

#set(hatch_core_sources
#  hatch/core/memory.hh
#  hatch/core/memory_fwd.hh
#  hatch/core/pointer.hh
#  hatch/core/pointer_impl.hh
#  hatch/core/handle.hh
#  hatch/core/handle_impl.hh
#  hatch/core/allocated.hh
#  hatch/core/allocated_impl.hh
#  hatch/core/liberated.hh
#  hatch/core/liberated_impl.hh
#  hatch/core/allocator.hh
#  hatch/core/allocator_impl.hh
#
#  hatch/core/async.hh
#  hatch/core/async_fwd.hh
#  hatch/core/promise.hh
#  hatch/core/promise_impl.hh
#  hatch/core/future.hh
#  hatch/core/future_impl.hh

#  hatch/core/streamer.hh
#  hatch/core/streamer.cc
#  hatch/core/socket.hh
#  hatch/core/reactor.hh
#  hatch/core/reactor.cc
#)

#add_library(hatch_core STATIC ${hatch_core_sources})
#set_property(TARGET hatch_core PROPERTY CXX_STANDARD 17)

########
# test #
########

set(hatch_utility_test_sources
  test/utility/meta.cc
  test/utility/integral.cc
  test/utility/container.cc
  test/utility/pointed.cc
  test/utility/indexed.cc
  test/utility/chain.cc
  test/utility/branch.cc
#  test/utility/owning.cc
#  test/utility/list.cc
#  test/utility/tree.cc
)

add_executable(hatch_utility_test ${hatch_utility_test_sources})
set_property(TARGET hatch_utility_test PROPERTY CXX_STANDARD 20)
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
#set_property(TARGET hatch_core_test PROPERTY CXX_STANDARD 20)
#target_link_libraries(hatch_core_test gtest_main)
#
#add_test(NAME hatch_core_test COMMAND hatch_core_test)
