/**
 * @file
 * RPC Package functional tester implementation file.
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

#include <gtest/gtest.h>
#include <rpc_pkg.h>
#include <testclientstub.h>
#include <testserverstub.h>

typedef rpc::
    jsonrpc_remote_object<rpc_stubs::cTestClientStub, rpc::http::cJSONClientConnector, std::string>
        cTestClient;

class cTestServer : public rpc::jsonrpc_object_server<rpc_stubs::cTestServerStub>
{
public:
    cTestServer(rpc::http::cJSONRPCServer& oServer) : m_oServer(oServer)
    {
    }

    virtual int GetInteger(int nValue)
    {
        return nValue;
    }

    virtual std::string Concat(const std::string& strString1, const std::string& strString2)
    {
        return strString1 + strString2;
    }

    virtual std::string GetIntegerAsString(const std::string& nValue)
    {
        return to_string(stoll(nValue));
    }

    rpc::Result GenerateError()
    {
        RETURN_ERROR_DESCRIPTION(-2, "test error");
    }

    virtual Json::Value GetResult()
    {
        return result_to_json(GenerateError());
    }

    virtual Json::Value RegisterObject()
    {
        m_pChildServer.reset(new cTestServer(m_oServer));
        return result_to_json(m_oServer.RegisterRPCObject("test_register", m_pChildServer.get()));
    }

    virtual Json::Value UnregisterObject()
    {
        return result_to_json(m_oServer.UnregisterRPCObject("test_register"));
        m_pChildServer.release();
    }

    virtual Json::Value UnregisterSelf()
    {
        // this deadlocks
        return result_to_json(m_oServer.UnregisterRPCObject("test"));
    }

private:
    rpc::http::cJSONRPCServer& m_oServer;
    std::auto_ptr<cTestServer> m_pChildServer;
};

/**
 * @req_id #34310
 */
TEST(cTesterPkgRpc, TestCalls)
{
    rpc::http::cJSONRPCServer rpc_server;
    cTestServer oTestServer(rpc_server);
    ASSERT_TRUE(a_util::result::isOk(rpc_server.RegisterRPCObject("test", &oTestServer)));
    ASSERT_TRUE(a_util::result::isOk(rpc_server.StartListening("http://127.0.0.1:1234")));

    cTestClient oClient("http://127.0.0.1:1234/test");

    ASSERT_TRUE(oClient.GetInteger(1234) == 1234);
    ASSERT_TRUE(oClient.Concat("foo", "bar") == "foobar");

    std::string strValue = rpc::cJSONConversions::to_string(std::numeric_limits<int64_t>::max());
    ASSERT_TRUE(oClient.GetIntegerAsString(strValue) == strValue);

    a_util::result::Result nResult = rpc::cJSONConversions::json_to_result(oClient.GetResult());

    // LOG_RESULT(nResult);
    ASSERT_TRUE(nResult.getErrorCode() == a_util::result::Result(-2).getErrorCode());
    ASSERT_TRUE(a_util::strings::isEqual(nResult.getDescription(), "test error"));

    ASSERT_TRUE(std::string(nResult.getFunction()).find("GenerateError") != std::string::npos);
    ASSERT_TRUE(a_util::strings::isEqual(nResult.getFile(), __FILE__));
    ASSERT_TRUE(nResult.getLine() > 0 && nResult.getLine() < __LINE__);

    ASSERT_TRUE(isOk(rpc::cJSONConversions::json_to_result(oClient.RegisterObject())));
    {
        cTestClient oRegisteredObjectClient("http://127.0.0.1:1234/test_register");
        ASSERT_TRUE(oRegisteredObjectClient.GetInteger(1234) == 1234);
    }
    ASSERT_TRUE(isOk(rpc::cJSONConversions::json_to_result(oClient.UnregisterObject())));

    ASSERT_TRUE(isOk(rpc_server.UnregisterRPCObject("test")));
}

/**
 * @req_id"#34310
 */
TEST(cTesterPkgRpc, TestObjectNameWithSpaces)
{
#define TEST_OBJ_STRING "test with spaces/and even more spaces"
    rpc::http::cJSONRPCServer rpc_server;
    cTestServer oTestServer(rpc_server);
    ASSERT_TRUE(isOk(rpc_server.RegisterRPCObject(TEST_OBJ_STRING, &oTestServer)));
    ASSERT_TRUE(isOk(rpc_server.StartListening("http://127.0.0.1:1234")));

    cTestClient oClient("http://127.0.0.1:1234/" TEST_OBJ_STRING);
    ASSERT_TRUE(oClient.GetInteger(1234) == 1234);
}
