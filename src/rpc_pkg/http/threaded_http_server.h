/**
 * @file
 * RPC Launcher.
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

#include <a_util/result/result_type.h>
#include <a_util/memory.h>

#ifndef PKG_RPC_RPC_DETAIL_THREAD_HTTP_SERVER_H_
#define PKG_RPC_RPC_DETAIL_THREAD_HTTP_SERVER_H_

namespace rpc
{
namespace http
{
namespace detail
{

class cThreadedHttpServer
{
public:
    cThreadedHttpServer();
    ~cThreadedHttpServer();

    /**
     * Starts listening and processing of requests.
     * @param[in] strURL The URL, i.e. http://0.0.0.0:8000
     * @return Standard result
     */
    a_util::result::Result StartListening(const char* strURL);

    /**
     * Stop processing of requests.
     * @return
     */
    a_util::result::Result StopListening();

protected:
    virtual bool HandleRequest(const std::string& strUrl,
                               const std::string& strRequest,
                               std::string& strResponse,
                               std::string& strContentType) = 0;

private:
    class cImplementation;
    a_util::memory::unique_ptr<cImplementation> m_pImplementation;
};

} // namespace detail
} // namespace http
} // namespace rpc

#endif
