#
# Copyright @ 2019 Audi AG. All rights reserved.
# 
#     This Source Code Form is subject to the terms of the Mozilla
#     Public License, v. 2.0. If a copy of the MPL was not distributed
#     with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
# 
# If it is not possible or desirable to put the notice in a particular file, then
# You may include the notice in a location (such as a LICENSE file in a
# relevant directory) where a recipient would be likely to look for such a notice.
# 
# You may add additional accurate notices of copyright ownership.
#

if(CMAKE_CROSSCOMPILING)
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules" "${CMAKE_MODULE_PATH}")
    find_package(JsonStubGenerator REQUIRED)
endif()

if(NOT BUILD_PKG_RPC)   ##standalone
    cmake_minimum_required(VERSION 3.0.0)   #minimum for generated find scripts
    project(pkg_rpc_tests)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    # if Conan is used, we have to include the generated file and doing some basic setup
    if(CONAN_COMPILER)
        if ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
            include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
        elseif ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo.cmake)
            include(${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo.cmake)
        elseif ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
            include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
        elseif ( EXISTS ${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo_multi.cmake)
            include(${CMAKE_CURRENT_BINARY_DIR}/../conanbuildinfo_multi.cmake)
        else()
            message(FATAL_ERROR "Conan build info can't be found.")
        endif()
    
        if(CORTEX_WORKSPACE)
            conan_basic_setup(TARGETS)
        else()
            conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
        endif()
        set(gtest_search_mode CONFIG)
    endif()

    if(CMAKE_CROSSCOMPILING)
        set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules" "${CMAKE_MODULE_PATH}")
        find_package(JsonStubGenerator REQUIRED)
    else()
        enable_testing()
    endif()

    find_package(pkg_rpc REQUIRED)
else()
    include(../cmake/stub_generation.cmake)
endif()

find_package(GTest REQUIRED ${gtest_search_mode}) 
add_subdirectory(function/common)
add_subdirectory(rpc/src)
