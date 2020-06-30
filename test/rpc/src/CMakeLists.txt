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
jsonrpc_generate_client_stub(${CMAKE_CURRENT_SOURCE_DIR}/test.json 
                             rpc_stubs::cTestClientStub ${CMAKE_CURRENT_BINARY_DIR}/testclientstub.h)
jsonrpc_generate_server_stub(${CMAKE_CURRENT_SOURCE_DIR}/test.json
                             rpc_stubs::cTestServerStub ${CMAKE_CURRENT_BINARY_DIR}/testserverstub.h)

add_executable(pkg_rpc_tester_rpc tester_pkg_rpc.cpp
                                  test.json
                                  ${CMAKE_CURRENT_BINARY_DIR}/testclientstub.h
                                  ${CMAKE_CURRENT_BINARY_DIR}/testserverstub.h)
add_test(pkg_rpc_tester_rpc
         pkg_rpc_tester_rpc
         WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../")
set_target_properties(pkg_rpc_tester_rpc PROPERTIES FOLDER "pkg_rpc/test")
target_include_directories(pkg_rpc_tester_rpc PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(pkg_rpc_tester_rpc PRIVATE pkg_rpc GTest::Main)
if(QNXNTO)
    target_link_libraries(pkg_rpc_tester_rpc PUBLIC socket)
endif()
