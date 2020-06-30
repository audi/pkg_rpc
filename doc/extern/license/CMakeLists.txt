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
if(NOT AEV_PRODUCT_FULL_NAME)
    # message(FATAL_ERROR "You have to specify a AEV_PRODUCT_FULL_NAME for license generation.")
    set(AEV_PRODUCT_FULL_NAME "PKG_RPC")
endif(NOT AEV_PRODUCT_FULL_NAME)

install(FILES MPL2.0.txt licenses.txt DESTINATION doc/license)

if(NOT EXCLUDE_PRODUCT_LICENSES)
    set(LICENSE_FILES_EXTENSION "* [licenses.txt](../license/licenses.txt)\n")
endif()

if(EXISTS ${CURRENT_IMPORTS_DIR}/licenses/list_used_licenses.md.in) 
    configure_file(${CURRENT_IMPORTS_DIR}/licenses/list_used_licenses.md.in 
                   ${CURRENT_IMPORTS_DIR}/licenses/list_used_licenses.md)
endif()
