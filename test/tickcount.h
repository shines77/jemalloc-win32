/*
    Copyright 2005-2011 Intel Corporation.  All Rights Reserved.

    This file is part of Threading Building Blocks.

    Threading Building Blocks is free software; you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    version 2 as published by the Free Software Foundation.

    Threading Building Blocks is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Threading Building Blocks; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    As a special exception, you may use this file as part of a free software
    library without restriction.  Specifically, if other files instantiate
    templates or use macros or inline functions from this file, or you compile
    this file and link it with other files to produce an executable, this
    file does not by itself cause the resulting executable to be covered by
    the GNU General Public License.  This exception does not however
    invalidate any other reasons why the executable file might be covered by
    the GNU General Public License.
*/

#ifndef _GMTL_TICKCOUNT_H_
#define _GMTL_TICKCOUNT_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//#include <gmtl/gmtl_stddef.h>
#include <windows.h>

#if _WIN32||_WIN64
//#include <gmtl/machine/windows_api.h>
#include <time.h>
#elif __linux__
#include <ctime>
#else /* generic Unix */
#include <sys/time.h>
#endif /* (choice of OS) */

namespace gmtl {

//! Absolute timestamp
/** @ingroup timing */
class tickcount
{
private:
    int64_t my_count;

public:
    //! Relative time interval.
    class interval {
        int64_t my_value;
        explicit interval( int64_t _value ) : my_value(_value) {}
    public:
        //! Construct a time interval representing zero time duration
        interval() : my_value(0) {};

        //! Construct a time interval representing sec seconds time duration
        explicit interval( double sec );

        //! Return the length of a time interval in seconds
        double seconds() const;
        double m_secs()  const;
        double u_secs()  const;

        friend class gmtl::tickcount;

        //! Extract the intervals from the tickcount_ts and subtract them.
        friend interval operator-( const tickcount& t1, const tickcount& t0 );

        //! Add two intervals.
        friend interval operator+( const interval& i, const interval& j ) {
            return interval(i.my_value + j.my_value);
        }

        //! Subtract two intervals.
        friend interval operator-( const interval& i, const interval& j ) {
            return interval(i.my_value - j.my_value);
        }

        //! Accumulation operator
        interval& operator+=( const interval& i ) { my_value += i.my_value; return *this; }

        //! Subtraction operator
        interval& operator-=( const interval& i ) { my_value -= i.my_value; return *this; }
    };
    
    //! Construct an absolute timestamp initialized to zero.
    tickcount() : my_count(0) {};

    void                reset( void );
    tickcount           begin( void );
    tickcount::interval end( void );

    //! Return current time.
    static tickcount    now( void );
    
    //! Subtract two timestamps to get the time interval between
    friend interval operator-( const tickcount& t1, const tickcount& t0 );

    tickcount& operator=( const tickcount& t );
};

inline void tickcount::reset( void ) {
    tickcount result;
    result = tickcount::now();
    my_count = result.my_count;
    return;
}

inline tickcount tickcount::begin( void ) {
    tickcount result;
    result = tickcount::now();
    my_count = result.my_count;
    return result;
}

inline tickcount::interval tickcount::end( void ) {
    interval result;
    tickcount nowtime = tickcount::now();
    result = tickcount::interval(nowtime - (const tickcount)*this);
    return result;
}

inline tickcount tickcount::now( void ) {
    tickcount result;
#if _WIN32||_WIN64
    result.my_count = GetTickCount();
#elif __linux__
    struct timespec ts;
#if TBB_USE_ASSERT
    int status = 
#endif /* TBB_USE_ASSERT */
        clock_gettime( CLOCK_REALTIME, &ts );
    _DOL_ASSERT( status==0, "CLOCK_REALTIME not supported" );
    result.my_count = static_cast<int64_t>(1000000000UL)*static_cast<int64_t>(ts.tv_sec) + static_cast<int64_t>(ts.tv_nsec);
#else /* generic Unix */
    struct timeval tv;
#if TBB_USE_ASSERT
    int status = 
#endif /* TBB_USE_ASSERT */
        gettimeofday(&tv, NULL);
    _DOL_ASSERT( status==0, "gettimeofday failed" );
    result.my_count = static_cast<int64_t>(1000000)*static_cast<int64_t>(tv.tv_sec) + static_cast<int64_t>(tv.tv_usec);
#endif /*(choice of OS) */
    return result;
}

inline tickcount::interval::interval( double sec )
{
#if _WIN32||_WIN64
    my_value = GetTickCount();
#elif __linux__
    my_value = static_cast<int64_t>(sec * 1E9);
#else /* generic Unix */
    my_value = static_cast<int64_t>(sec * 1E6);
#endif /* (choice of OS) */
}

inline tickcount::interval operator-( const tickcount& t1, const tickcount& t0 ) {
    return tickcount::interval( t1.my_count - t0.my_count );
}

inline double tickcount::interval::seconds( void ) const {
#if _WIN32||_WIN64
    return ((double)my_value / (double)CLK_TCK);
#elif __linux__
    return my_value * 1E-9;
#else /* generic Unix */
    return my_value * 1E-6;
#endif /* (choice of OS) */
}

inline double tickcount::interval::m_secs( void ) const {
#if _WIN32||_WIN64
    return ((double)my_value / (double)CLK_TCK) * 1E3;
#elif __linux__
    return my_value * 1E-6;
#else /* generic Unix */
    return my_value * 1E-3;
#endif /* (choice of OS) */
}

inline double tickcount::interval::u_secs( void ) const {
#if _WIN32||_WIN64
    return ((double)my_value / (double)CLK_TCK) * 1E6;
#elif __linux__
    return my_value * 1E-3;
#else /* generic Unix */
    return my_value * 1.0;
#endif /* (choice of OS) */
}

inline tickcount& tickcount::operator=( const tickcount& t ) {
    my_count = t.my_count;
    return *this;
}

}  // namespace gmtl

#endif /* _GMTL_TICKCOUNT_H_ */
