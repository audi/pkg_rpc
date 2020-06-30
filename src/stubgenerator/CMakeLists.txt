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
find_package(a_util 5.6.1 COMPONENTS strings REQUIRED)

add_executable(jsonrpcstub $<TARGET_OBJECTS:libjson-rpc-cpp-stubgenerator>
                           $<TARGET_OBJECTS:libjson-rpc-cpp>
                           $<TARGET_OBJECTS:jsoncpp>
                           stubgeneratorfactory.cpp
                           commandline.cpp
                           parserhelper.cpp
                           commandline.h
                           parserhelper.h)
get_target_property(LIBJSON_RPC_CPP_STUBGENERATOR_INCLUDE_DIRECTORIES libjson-rpc-cpp-stubgenerator
                    INCLUDE_DIRECTORIES)
target_include_directories(jsonrpcstub 
                           PRIVATE ${LIBJSON_RPC_CPP_STUBGENERATOR_INCLUDE_DIRECTORIES}
                                   ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(jsonrpcstub PRIVATE a_util_strings $<$<PLATFORM_ID:Linux>:pthread>)
target_compile_features(jsonrpcstub PRIVATE cxx_std_11)
set_target_properties(jsonrpcstub PROPERTIES FOLDER pkg_rpc/executable)

if (pkg_rpc_cmake_enable_installation)
    install(TARGETS jsonrpcstub DESTINATION bin CONFIGURATIONS Release RelWithDebInfo Debug)
endif (pkg_rpc_cmake_enable_installation)
