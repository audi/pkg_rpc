/**
 * @file
 * RPC Protocol implementation.
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

#include "rpc_pkg/rpc_object_registry.h"
#include "rpc_pkg/impl/rpc_lock_helper.h"

namespace rpc
{

cRPCObjectsRegistry::cRPCObjectsRegistry()
{
}

cRPCObjectsRegistry::~cRPCObjectsRegistry()
{
}

Result cRPCObjectsRegistry::RegisterRPCObject(const char* strName, IRPCObject* pObject)
{
    std::string strLocalName(strName);
    detail::lock_guard<a_util::concurrency::shared_mutex> oGuard(m_oObjectsLock);
    tRPCObjects::iterator itExisting = m_oRPCObjects.find(strLocalName);

    if (itExisting != m_oRPCObjects.end())
    {
        RETURN_ERROR_DESCRIPTION(
            AlreadyRegistered, "RPC-Registry: Object '%s' already registered.", strName);
    }

    m_oRPCObjects[strLocalName].first.reset(new a_util::concurrency::shared_mutex);
    m_oRPCObjects[strLocalName].second = pObject;
    return Result();
}

a_util::result::Result cRPCObjectsRegistry::UnregisterRPCObject(const char* strName)
{
    detail::lock_guard<a_util::concurrency::shared_mutex> oGuard(m_oObjectsLock);
    tRPCObjects::iterator itExisting = m_oRPCObjects.find(strName);

    if (itExisting == m_oRPCObjects.end())
    {
        RETURN_ERROR_DESCRIPTION(NotFound, "RPC-Registry: Object '%s' not found.", strName);
    }

    // make sure no one is using it anymore.
    // mind that an object cannot be unregistered in its own method call
    {
        detail::lock_guard<a_util::concurrency::shared_mutex> oObjectGuard(
            *itExisting->second.first);
    }

    m_oRPCObjects.erase(itExisting);

    return Result();
}

cRPCObjectsRegistry::cLockedRPCObject cRPCObjectsRegistry::GetRPCObject(const char* strName) const
{
    tRPCObjects::const_iterator itObject;
    {
        detail::shared_lock<a_util::concurrency::shared_mutex> oGuard(m_oObjectsLock);
        itObject = m_oRPCObjects.find(strName);
        if (itObject == m_oRPCObjects.end())
        {
            return cLockedRPCObject();
        }
        return cLockedRPCObject(itObject->second);
    }
}

} // namespace rpc
