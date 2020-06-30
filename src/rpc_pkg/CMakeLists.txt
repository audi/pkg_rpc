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
set(PKG_VERSION_LIBNAME ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR})
set(RPC_PUBLIC_HEADER_FILES rpc_server.h 
                            json_rpc.h
                            rpc_object_registry.h)
set(RPC_HTTPSERVER_PUBLIC_HEADER_FILES http/threaded_http_server.h
                                       http/http_rpc_server.h
                                       http/json_http_rpc.h)

add_library(${PROJECT_NAME} STATIC ../rpc_pkg.h
                                   ${RPC_PUBLIC_HEADER_FILES}
                                   ${RPC_HTTPSERVER_PUBLIC_HEADER_FILES}
                                   http/http_rpc_server.cpp
                                   http/json_http_rpc.cpp
                                   http/threaded_http_server.cpp
                                   impl/json_rpc.cpp
                                   impl/rpc_lock_helper.h
                                   impl/rpc_object_registry.cpp
                                   impl/url.h
                                   impl/url.cpp
                                   $<TARGET_OBJECTS:jsoncpp>
                                   $<TARGET_OBJECTS:libjson-rpc-cpp>)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

if(MSVC)
    get_target_property(DEB_POSTFIX ${PROJECT_NAME} DEBUG_POSTFIX)
    get_target_property(REL_POSTFIX ${PROJECT_NAME} RELWITHDEBINFO_POSTFIX)
    set_target_properties(${PROJECT_NAME} PROPERTIES 
                          PDB_OUTPUT_DIRECTORY_DEBUG
                            ${CMAKE_CURRENT_BINARY_DIR}/${${PROJECT_NAME}_BUILD_NAME_}Debug
                          PDB_OUTPUT_NAME_DEBUG ${PROJECT_NAME}${DEB_POSTFIX}.pdb
                          PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO
                            ${CMAKE_CURRENT_BINARY_DIR}/${${PROJECT_NAME}_BUILD_NAME_}RelWithDebInfo
                          PDB_OUTPUT_NAME_RELWITHDEBINFO ${PROJECT_NAME}${REL_POSTFIX}.pdb
                          COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG
                            ${CMAKE_CURRENT_BINARY_DIR}/${${PROJECT_NAME}_BUILD_NAME_}Debug
                          COMPILE_PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO
                            ${CMAKE_CURRENT_BINARY_DIR}/${${PROJECT_NAME}_BUILD_NAME_}RelWithDebInfo
                          COMPILE_PDB_NAME_DEBUG ${PROJECT_NAME}${DEB_POSTFIX}
                          COMPILE_PDB_NAME_RELWITHDEBINFO ${PROJECT_NAME}${REL_POSTFIX}
                        )
endif(MSVC)

find_package(a_util 5.6.1 COMPONENTS result regex REQUIRED)

get_target_property(JSONCPP_INCLUDE_DIRECTORIES jsoncpp INCLUDE_DIRECTORIES)
get_target_property(LIBJSON_RPC_CPP_INCLUDE_DIRECTORIES libjson-rpc-cpp INCLUDE_DIRECTORIES)
get_target_property(HTTPLIB_INCLUDE_DIRECTORIES httplib INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(${PROJECT_NAME} 
                           PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
                                  ${JSONCPP_INCLUDE_DIRECTORIES}
                                  ${LIBJSON_RPC_CPP_INCLUDE_DIRECTORIES}
                                  ${HTTPLIB_INCLUDE_DIRECTORIES}
                                  $<INSTALL_INTERFACE:include>)
target_link_libraries(${PROJECT_NAME} PUBLIC a_util_result
                                      PRIVATE a_util_regex
                                              $<$<CXX_COMPILER_ID:MSVC>:ws2_32.lib>)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11) # C++11 for self and dependants
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER pkg_rpc/library)

if (pkg_rpc_cmake_enable_installation)
    message(STATUS "${PROJECT_NAME} will be installed to directory: ${CMAKE_INSTALL_PREFIX}")
    install(FILES ${RPC_PUBLIC_HEADER_FILES} DESTINATION include/rpc_pkg)
    install(FILES ../rpc_pkg.h DESTINATION include)
    install(FILES ${RPC_HTTPSERVER_PUBLIC_HEADER_FILES} DESTINATION include/rpc_pkg/http)
    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} ARCHIVE DESTINATION lib)
    if(MSVC)
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${${PROJECT_NAME}_BUILD_NAME_}Debug/${PROJECT_NAME}${DEB_POSTFIX}.pdb
                CONFIGURATIONS Debug
                DESTINATION lib)
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${${PROJECT_NAME}_BUILD_NAME_}RelWithDebInfo/${PROJECT_NAME}${REL_POSTFIX}.pdb
                CONFIGURATIONS RelWithDebInfo
                DESTINATION lib)
    endif(MSVC)

    ##Packaging
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/cmake/pkg_rpcConfigVersion.cmake
                                     COMPATIBILITY AnyNewerVersion)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/pkg_rpcConfigVersion.cmake DESTINATION cmake)
    install(EXPORT ${PROJECT_NAME} FILE pkg_rpcTargets.cmake DESTINATION cmake)
else (pkg_rpc_cmake_enable_installation)
    message(WARNING "${PROJECT_NAME} won't be installed ('pkg_rpc_cmake_enable_installation=OFF')")
endif (pkg_rpc_cmake_enable_installation)
