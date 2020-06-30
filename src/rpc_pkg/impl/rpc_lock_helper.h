/**
 * @file
 * RPC Helper
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

#ifndef PKG_RPC_HELPER_INCLUDED
#define PKG_RPC_HELPER_INCLUDED

namespace rpc
{
namespace detail
{
template <typename Mutex>
class lock_guard
{
public:
    lock_guard(Mutex& oMutex) : m_oMutex(oMutex)
    {
        oMutex.lock();
    }

    ~lock_guard()
    {
        m_oMutex.unlock();
    }

private:
    Mutex& m_oMutex;
};

template <typename Mutex>
class shared_lock
{
public:
    shared_lock(Mutex& oMutex) : m_oMutex(oMutex)
    {
        oMutex.lock_shared();
    }

    ~shared_lock()
    {
        m_oMutex.unlock_shared();
    }

private:
    Mutex& m_oMutex;
};
} // namespace detail

} // namespace rpc

#endif // PKG_RPC_HELPER_INCLUDED
