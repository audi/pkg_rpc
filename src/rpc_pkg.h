/**
 * @file
 * RPC Package Header
 *
 * @copyright
 * @verbatim
   Copyright @ 2020 AUDI AG. All rights reserved.

       This Source Code Form is subject to the terms of the Mozilla
       Public License, v. 2.0. If a copy of the MPL was not distributed
       with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

   If it is not possible or desirable to put the notice in a particular file, then
   You may include the notice in a location (such as a LICENSE file in a
   relevant directory) where a recipient would be likely to look for such a notice.

   You may add additional accurate notices of copyright ownership.
   @endverbatim
 */

#ifndef PKG_RPC_RPC_PKG_H_INCLUDED
#define PKG_RPC_RPC_PKG_H_INCLUDED

#include <a_util/result.h>
#include <a_util/strings.h>
#include <a_util/concurrency.h>

#include "rpc_pkg/rpc_server.h"
#include "rpc_pkg/rpc_object_registry.h"

// http
#include "rpc_pkg/http/threaded_http_server.h"
#include "rpc_pkg/http/http_rpc_server.h"
#include "rpc_pkg/http/json_http_rpc.h"

// common
#include "rpc_pkg/json_rpc.h"

#endif // PKG_RPC_RPC_PKG_H_INCLUDED
