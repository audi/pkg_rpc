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
message(STATUS "3rdparty CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")

add_subdirectory(jsoncpp)
add_subdirectory(libjson-rpc-cpp)

add_library(httplib INTERFACE)
target_include_directories(httplib INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cpp-httplib>)
