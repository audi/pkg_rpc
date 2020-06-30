/**
 * @file
 * RPC Protocol declaration.
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

#ifndef PKG_RPC_JSON_HTTP_H_INCLUDED
#define PKG_RPC_JSON_HTTP_H_INCLUDED

#include <jsonrpccpp/client/iclientconnector.h>
#include <jsonrpccpp/server/abstractserverconnector.h>
#include "http_rpc_server.h"

namespace rpc
{

namespace http
{

class cJSONRPCServer : public detail::cRPCServer
{
public:
    cJSONRPCServer() : cRPCServer("application/json")
    {
    }
};

/**
 * Connector that sends RPC messages via HTTP
 */
class cJSONClientConnector : public jsonrpc::IClientConnector
{

public:
    /**
     * Constructor
     * @param[in] strUrl The HTTP url, i.e. http://localhost:8000/system
     */
    cJSONClientConnector(const std::string& strUrl);
    ~cJSONClientConnector();

public:
    void SendRPCMessage(const std::string& message,
                        std::string& result) throw(jsonrpc::JsonRpcException);

private:
    class cImplementation;
    cImplementation* m_pImplementation;
};

} // namespace http
} // namespace rpc

#endif // PKG_RPC_JSON_HTTP_H_INCLUDED
