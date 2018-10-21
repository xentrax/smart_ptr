#ifndef BOOST_SMART_PTR_DETAIL_LWM_WIN32_CS_HPP_INCLUDED
#define BOOST_SMART_PTR_DETAIL_LWM_WIN32_CS_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  boost/detail/lwm_win32_cs.hpp
//
//  Copyright (c) 2002, 2003 Peter Dimov
//  Copyright (c) Microsoft Corporation 2014
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/predef.h>

#include <boost/winapi/critical_section.hpp>

namespace boost
{

namespace detail
{

class lightweight_mutex
{
private:

    boost::winapi::CRITICAL_SECTION_ cs_;

    lightweight_mutex(lightweight_mutex const &);
    lightweight_mutex & operator=(lightweight_mutex const &);

public:

    lightweight_mutex()
    {
#if BOOST_PLAT_WINDOWS_RUNTIME
        boost::winapi::InitializeCriticalSectionEx(&cs_, 4000, 0);
#else
        boost::winapi::InitializeCriticalSection(&cs_);
#endif
    }

    ~lightweight_mutex()
    {
        boost::winapi::DeleteCriticalSection(&cs_);
    }

    class scoped_lock;
    friend class scoped_lock;

    class scoped_lock
    {
    private:

        lightweight_mutex & m_;

        scoped_lock(scoped_lock const &);
        scoped_lock & operator=(scoped_lock const &);

    public:

        explicit scoped_lock(lightweight_mutex & m): m_(m)
        {
            boost::winapi::EnterCriticalSection(&m_.cs_);
        }

        ~scoped_lock()
        {
            boost::winapi::LeaveCriticalSection(&m_.cs_);
        }
    };
};

} // namespace detail

} // namespace boost

#endif // #ifndef BOOST_SMART_PTR_DETAIL_LWM_WIN32_CS_HPP_INCLUDED
