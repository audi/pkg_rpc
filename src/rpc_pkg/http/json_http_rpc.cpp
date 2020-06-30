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

#include "rpc_pkg/http/json_http_rpc.h"
#include "rpc_pkg/impl/url.h"
#include "httplib.h"

namespace rpc
{
namespace http
{
namespace detail
{

#ifdef WIN32
#define snprintf _snprintf
#endif
// this is not the same as httplib::detail::encode_url as it treats spaces differently
std::string url_encode(const std::string& s)
{
    std::string result;

    for (int i = 0; s[i]; i++)
    {
        switch (s[i])
        {
            case ' ':
                result += "%20";
                break;
            case '\'':
                result += "%27";
                break;
            case ',':
                result += "%2C";
                break;
            case ':':
                result += "%3A";
                break;
            case ';':
                result += "%3B";
                break;
            default:
                if (s[i] < 0)
                {
                    result += '%';
                    char hex[4];
                    size_t len = snprintf(hex, sizeof(hex), "%02X", (unsigned char) s[i]);
                    assert(len == 2);
                    result.append(hex, len);
                }
                else
                {
                    result += s[i];
                }
                break;
        }
    }

    return result;
}

std::string encode_url_path(const std::string& strUrl)
{
    size_t nSlashPosition = strUrl.find('/');
    if (nSlashPosition == std::string::npos)
    {
        return strUrl;
    }
    nSlashPosition = strUrl.find('/', nSlashPosition + 1);
    if (nSlashPosition == std::string::npos)
    {
        return strUrl;
    }
    nSlashPosition = strUrl.find('/', nSlashPosition + 1);
    if (nSlashPosition == std::string::npos)
    {
        return strUrl;
    }
    return strUrl.substr(0, nSlashPosition) + url_encode(strUrl.substr(nSlashPosition));
}

} // namespace detail

class cJSONClientConnector::cImplementation
{
public:
    rpc::cUrl m_oUrl;
    httplib::Client m_oHttpClient;

public:
    cImplementation(const std::string& strUrl)
        : m_oUrl(detail::encode_url_path(strUrl).c_str()),
          m_oHttpClient(m_oUrl.GetAuthority().GetHost().c_str(), m_oUrl.GetAuthority().GetPort())
    {
    }
};

cJSONClientConnector::cJSONClientConnector(const std::string& strUrl)
    : m_pImplementation(new cImplementation(strUrl))
{
}

cJSONClientConnector::~cJSONClientConnector()
{
    delete m_pImplementation;
}

void cJSONClientConnector::SendRPCMessage(const std::string& message,
                                          std::string& result) throw(jsonrpc::JsonRpcException)
{
    const std::string url = m_pImplementation->m_oUrl.GetPath().insert(0, 1, '/');
    const char* const content_type = "application/json";

    httplib::Client& http_client = m_pImplementation->m_oHttpClient;
    typedef a_util::memory::unique_ptr<httplib::Response> Response;
    Response response(http_client.post(url.c_str(), message, content_type));

    if (!response.get())
    {
        throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_CONNECTOR,
                                        "error while performing call, invalid response received");
    }

    if (response->status != 200)
    {
        using a_util::strings::format;
        throw jsonrpc::JsonRpcException(
            jsonrpc::Errors::ERROR_CLIENT_CONNECTOR,
            format("http error while performing call: %d", response->status));
    }

#if A_UTILS_VERSION_MAJOR < 4
    result = response->body;
#else
    result = std::move(response->body);
#endif
}

} // namespace http
} // namespace rpc
