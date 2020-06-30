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

#ifndef PKG_RPC_OBJECTSERVER_REGISTRY_H_INCLUDED
#define PKG_RPC_OBJECTSERVER_REGISTRY_H_INCLUDED

#include <a_util/result.h>
#include <a_util/concurrency.h>
#include <a_util/memory.h>
#include <map>
#include "rpc_pkg/rpc_server.h"

namespace rpc
{

/**
 * An RPC Server that receives calls via HTTP.
 */
class cRPCObjectsRegistry : public IRPCObjectsRegistry
{
public:
    /**
     * Constructor.
     * @param[in] strContentType The content type that should be set for the responses.
     */
    cRPCObjectsRegistry();

    /**
     * Destructor.
     */
    ~cRPCObjectsRegistry();

    /**
     * @copydoc IRPCServer::RegisterRPCObject
     */
    virtual Result RegisterRPCObject(const char* strName, IRPCObject* pObject);

    /**
     * @copydoc IRPCServer::UnregisterRPCObject
     */
    virtual Result UnregisterRPCObject(const char* strName);

    typedef std::pair<a_util::memory::unique_ptr<a_util::concurrency::shared_mutex>, IRPCObject*>
        tRPCItem;
    typedef std::pair<a_util::concurrency::shared_mutex*, IRPCObject*> tRPCRawItem;
    class cLockedRPCObject
    {
        tRPCRawItem m_oItem;

    public:
        cLockedRPCObject()
            : m_oItem(
                  std::make_pair<a_util::concurrency::shared_mutex*, IRPCObject*>(nullptr, nullptr))
        {
        }
        ~cLockedRPCObject()
        {
            if (m_oItem.first)
            {
                m_oItem.first->unlock_shared();
            }
        }
        cLockedRPCObject(const cLockedRPCObject& other) : m_oItem(other.m_oItem)
        {
            if (m_oItem.first)
            {
                m_oItem.first->lock_shared();
            }
        }
        cLockedRPCObject& operator=(const cLockedRPCObject& other)
        {
            m_oItem = other.m_oItem;
            if (m_oItem.first)
            {
                m_oItem.first->lock_shared();
            }
            return *this;
        }
        cLockedRPCObject(const tRPCItem& oItem)
            : m_oItem(std::make_pair(oItem.first.get(), oItem.second))
        {
            if (m_oItem.first)
            {
                m_oItem.first->lock_shared();
            }
        }
        IRPCObject* operator->()
        {
            return m_oItem.second;
        }
        operator bool() const
        {
            return (m_oItem.first != nullptr);
        }
    };

    virtual cLockedRPCObject GetRPCObject(const char* strName) const;

private:
    mutable a_util::concurrency::shared_mutex m_oObjectsLock;
    typedef std::map<std::string, tRPCItem> tRPCObjects;
    tRPCObjects m_oRPCObjects;
};

} // namespace rpc

#endif // PKG_RPC_OBJECTSERVER_REGISTRY_H_INCLUDED
