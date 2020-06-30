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
################################ Options and general project setup ################################
cmake_minimum_required(VERSION 3.8) # cxx_std_11
project(pkg_rpc VERSION 3.3.1)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

option(pkg_rpc_cmake_enable_installation
       "Enable installation of the pkg_rpc library to CMAKE_INSTALL_PREFIX (default: ON)"
       ON)
option(pkg_rpc_cmake_enable_tests "Enable tests - requires googletest (default: ON)" ON)
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
endif(CONAN_COMPILER)
###################################################################################################

add_subdirectory(3rdparty)
add_subdirectory(src)

if (pkg_rpc_cmake_enable_installation)
    configure_file(pkg_rpcConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/pkg_rpcConfig.cmake COPYONLY)
    configure_file(pkg_rpcConfigVersion.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/pkg_rpcConfigVersion.cmake COPYONLY)
    install(FILES Readme.md
	    	  ${CMAKE_CURRENT_BINARY_DIR}/pkg_rpcConfig.cmake
                  ${CMAKE_CURRENT_BINARY_DIR}/pkg_rpcConfigVersion.cmake
            DESTINATION .)
    install(DIRECTORY cmake DESTINATION .)
endif (pkg_rpc_cmake_enable_installation)

add_subdirectory(doc)
if (pkg_rpc_cmake_enable_tests)
    message(STATUS "pkg_rpc integrating tests from subdirectory 'test/'")
    if(CMAKE_CROSSCOMPILING)
        set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" "${CMAKE_MODULE_PATH}")
        find_package(JsonStubGenerator REQUIRED)
    else()
        enable_testing()
    endif()

    set(BUILD_PKG_RPC True)
    add_subdirectory(test)
endif (pkg_rpc_cmake_enable_tests)
