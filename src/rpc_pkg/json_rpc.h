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

#ifndef PKG_RPC_JSON_RPC_H_INCLUDED
#define PKG_RPC_JSON_RPC_H_INCLUDED

#include <jsonrpccpp/client/iclientconnector.h>
#include <jsonrpccpp/server/abstractserverconnector.h>

#if defined(__QNX__) && defined(__GNUC__) && (__GNUC__ == 5)
#include <string>
#include <sstream>
#include <cstdint>
#include <cstdlib>
#else
#define HAVE_STD_TO_STRING
#define HAVE_STD_STOLL
#endif

#include "rpc_server.h"

namespace rpc
{

/**
 * Utility base class to facilitate JSON conversions
 */
class cJSONConversions
{
public:
    static inline Json::Value result_to_json(a_util::result::Result nResult)
    {
        Json::Value oResult;
        oResult["ErrorCode"] = nResult.getErrorCode();
        oResult["Description"] = nResult.getDescription();
        oResult["Line"] = nResult.getLine();
        oResult["File"] = nResult.getFile();
        oResult["Function"] = nResult.getFunction();
        return oResult;
    }

    static inline std::string to_string(uint64_t nValue)
    {
#ifdef HAVE_STD_TO_STRING
        return std::to_string(nValue);
#else
        // Bug in g++ 5.x.x header file, not having std::to_string for C++11
        std::ostringstream stream;
        stream << nValue;
        return stream.str();
#endif
    }

    static inline std::string to_string(int64_t nValue)
    {
#ifdef HAVE_STD_TO_STRING
        return std::to_string(nValue);
#else
        // Bug in g++ 5.x.x header file, not having std::to_string for C++11
        std::ostringstream stream;
        stream << nValue;
        return stream.str();
#endif
    }

    static inline a_util::result::Result json_to_result(const Json::Value& oValue)
    {

        return a_util::result::Result(oValue["ErrorCode"].asInt(),
                                      oValue["Description"].asString().c_str(),
                                      oValue["Line"].asInt(),
                                      oValue["File"].asString().c_str(),
                                      oValue["Function"].asString().c_str());
    }

    static inline int64_t stoll(const std::string& strValue)
    {
#ifdef HAVE_STD_STOLL
        return std::stoll(strValue);
#else
        // Bug in g++ 5.x.x header file, not having std::sto(u)ll for C++11
        return strtoll(strValue.c_str(), 0, 10);
#endif
    }

    static inline uint64_t stoull(const std::string& strValue)
    {
#ifdef HAVE_STD_STOLL
        return std::stoull(strValue);
#else
        // Bug in g++ 5.x.x header file, not having std::sto(u)ll for C++11
        return strtoull(strValue.c_str(), 0, 10);
#endif
    }
};

/**
 * Use this template for direct use of the stub methods.
 */
template <typename Stub, typename Connector, typename ConnectorInitializer>
class jsonrpc_remote_object : private Connector, public Stub
{
public:
    /**
     * Constructor
     * @param[in] oInitializer a instance of the \tparam ConnectorInitializer
     *                         i.e. if using http then this is the HTTP url, i.e.
     * http://localhost:8000/system
     */
    jsonrpc_remote_object(const ConnectorInitializer& oInitializer)
        : Connector(oInitializer), Stub(*static_cast<jsonrpc::IClientConnector*>(this))
    {
    }

protected:
    /**
     * Access the rpc stub.
     * @return The stub.
     */
    Stub& GetStub() const
    {
        return *const_cast<Stub*>(static_cast<const Stub*>(this));
    }
};

/**
 * Template for mapping interfaces to a client stub.
 */
template <typename Stub, typename Interface, typename Connector, typename ConnectorInitializer>
class jsonrpc_remote_interface
    : private jsonrpc_remote_object<Stub, Connector, ConnectorInitializer>,
      protected cJSONConversions,
      public Interface
{
public:
    /**
     * Constructor
     * @param[in] strUrl The HTTP url, i.e. http://localhost:8000/system
     */
    jsonrpc_remote_interface(const ConnectorInitializer& oInitializer)
        : jsonrpc_remote_object<Stub, Connector, ConnectorInitializer>(oInitializer)
    {
    }

protected:
    /**
     * Access the rpc stub.
     * @return The stub.
     */
    Stub& GetStub() const
    {
        return *const_cast<Stub*>(static_cast<const Stub*>(this));
    }
};

/// macro to return a default value in case of an exception
#define EXCEPTION_TO_DEFAULT(__default, __body)                                                    \
    try                                                                                            \
    {                                                                                              \
        __body                                                                                     \
    }                                                                                              \
    catch (...)                                                                                    \
    {                                                                                              \
        return __default;                                                                          \
    }

/// macro to return a tResult in case of an exception
#if A_UTILS_VERSION_MAJOR < 4
#define EXCEPTION_TO_RESULT(__body)                                                                \
    try                                                                                            \
    {                                                                                              \
        __body                                                                                     \
    }                                                                                              \
    catch (...)                                                                                    \
    {                                                                                              \
        return a_util::result::Result(ERR_FAILED);                                                 \
    }
#else
#define EXCEPTION_TO_RESULT(__body)                                                                \
    try                                                                                            \
    {                                                                                              \
        __body                                                                                     \
    }                                                                                              \
    catch (const std::exception& oEx)                                                              \
    {                                                                                              \
        RETURN_ERROR_DESC(ERR_FAILED, "RPC call failed: %s", oEx.what());                          \
    }                                                                                              \
    catch (...)                                                                                    \
    {                                                                                              \
        RETURN_ERROR(ERR_FAILED);                                                                  \
    }
#endif

/**
 * Connector that sends responses via @ref IResponse
 */
class cServerConnector : public jsonrpc::AbstractServerConnector
{
public:
    bool StartListening()
    {
        return true;
    }

    bool StopListening()
    {
        return true;
    }

    bool SendResponse(const std::string& response, void* addInfo)
    {
        IResponse* pResponse = static_cast<IResponse*>(addInfo);
        pResponse->Set(response.data(), response.size());
        return true;
    }

    bool OnRequest(const std::string& request, IResponse* response)
    {
        std::string response_value;
        ProcessRequest(request, response_value);
        response->Set(response_value.c_str(), response_value.size());
        return true;
    }
};

/**
 * Template to implement an RPC object
 */
template <typename ServerStub, typename Connector = cServerConnector>
class jsonrpc_object_server : protected Connector,
                              protected ServerStub,
                              public IRPCObject,
                              protected cJSONConversions
{
public:
    jsonrpc_object_server() : ServerStub(*static_cast<jsonrpc::AbstractServerConnector*>(this))
    {
    }

public:
    virtual a_util::result::Result
        HandleCall(const char* strRequest, size_t nRequestSize, IResponse& oResponse)
    {
        try
        {
            std::string strRequestHelper(strRequest, nRequestSize);
            if (!Connector::OnRequest(strRequestHelper, &oResponse))
            {
                return Result(InvalidCall);
            }
        }
        catch (...)
        {
            return Result(FatalError);
        }

        return Result();
    }
};

} // namespace rpc

#endif // PKG_RPC_HTTP_RPC_PROTOCOL_H_INCLUDED
