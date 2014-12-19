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

#ifndef _JIMI_STOP_WATCH_H_
#define _JIMI_STOP_WATCH_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jemalloc_test.h"

#ifdef _MSC_VER
#include "msvc_compat/stdint.h"
#else
#include <stdint.h>
#endif

#if _WIN32 || _WIN64
#include <time.h>
#include <windows.h>
#elif __linux__
#include <ctime>
#else /* generic Unix */
#include <sys/time.h>
#endif /* (choice of OS) */

namespace jimi {

///////////////////////////////////////////////////////////////////////////
// stop_watch_base<T>
///////////////////////////////////////////////////////////////////////////

//! Absolute timestamp
/** @ingroup timing */
template<class T>
class stop_watch_base
{
public:
    typedef int64_t timestamp_t;

public:
    //! Construct an absolute timestamp initialized to zero.
    stop_watch_base() : startTime(0), stopTime(0) { };
    stop_watch_base(const stop_watch_base &src);

    stop_watch_base &operator = (const stop_watch_base &t);

    bool    isRunning(void);

    void    reset(void);

    //! restart() is equivalent to reset() and begin()
    void    restart(void);
    void    rebegin(void);

    void    start(void);
    void    stop(void);

    void    begin(void);
    void    end(void);

    //! Return current time().
    static timestamp_t  now(void);

    //! Return current time(double).
    static double       nowf(void);

    //! Return current time() [Millisecond].
    static timestamp_t  now_ms(void);

    static double       intervalSeconds(int64_t t1, int64_t t2);
    static double       intervalSeconds(double t1, double t2);

    //! Return current time() [Millisecond].
    static timestamp_t  currentTimeMillis(void);
    static double       currentTimeMillisf(void);

    timestamp_t getTimeStamp(void);

    timestamp_t getStartTime(void) { return startTime; };
    timestamp_t getStopTime(void)  { return stopTime;  };

    void    internal_getElapsedTime(timestamp_t &elapsedTime);

    double  getSeconds(void);
    double  getMillisec(void);
    double  getElapsedTime(void);

protected:
    static timestamp_t  internal_now();
    static double       internal_nowf();
    static timestamp_t  internal_now_ms();

    static void         internal_getTimeStamp(timestamp_t &result);

    void    internal_start();
    void    internal_stop();

protected:
    timestamp_t startTime, stopTime;
};

template<class T>
inline stop_watch_base<T>::stop_watch_base(const stop_watch_base &src)
{
    startTime   = src.startTime;
    stopTime    = src.stopTime;
}

template<class T>
inline stop_watch_base<T> &stop_watch_base<T>::operator = (const stop_watch_base &sw)
{
    startTime   = sw.startTime;
    stopTime    = sw.stopTime;
    return *this;
}

/* 单位: 根据各平台最小单位来决定, Windows: RTSC计数, tick; Linux: 纳秒, nsec; Unix: 微秒, usec; */
template<class T>
inline void stop_watch_base<T>::internal_getTimeStamp(timestamp_t &result)
{
#if _WIN32 || _WIN64
    LARGE_INTEGER qp_cnt;
    QueryPerformanceCounter(&qp_cnt);
    result = static_cast<timestamp_t>(qp_cnt.QuadPart);
#elif __linux__
    struct timespec ts;
#if JIMI_USE_ASSERT
    int status =
#endif /* JIMI_USE_ASSERT */
        clock_gettime(CLOCK_REALTIME, &ts);
    JIMI_ASSERT_EX(status == 0, "CLOCK_REALTIME not supported");
    result = static_cast<timestamp_t>(static_cast<int64_t>(1000000000UL) * static_cast<int64_t>(ts.tv_sec)
                                      + static_cast<int64_t>(ts.tv_nsec));
#else /* generic Unix */
    struct timeval tv;
#if JIMI_USE_ASSERT
    int status =
#endif /* JIMI_USE_ASSERT */
        gettimeofday(&tv, NULL);
    JIMI_ASSERT_EX(status == 0, "gettimeofday failed");
    result = static_cast<timestamp_t>(static_cast<int64_t>(1000000UL) * static_cast<int64_t>(tv.tv_sec)
                                      + static_cast<int64_t>(tv.tv_usec));
#endif /*(choice of OS) */
}

/* 单位: 纳秒, nsec */
template<class T>
inline typename stop_watch_base<T>::timestamp_t
stop_watch_base<T>::internal_now(void)
{
    timestamp_t result;

#if _WIN32 || _WIN64
    LARGE_INTEGER qp_cnt, qp_freq;
    QueryPerformanceCounter(&qp_cnt);
    QueryPerformanceFrequency(&qp_freq);
    result = static_cast<timestamp_t>(((double)qp_cnt.QuadPart / (double)qp_freq.QuadPart) * 1000000000.0);
#elif __linux__
    struct timespec ts;
#if JIMI_USE_ASSERT
    int status =
#endif /* JIMI_USE_ASSERT */
        clock_gettime(CLOCK_REALTIME, &ts);
    JIMI_ASSERT_EX(status == 0, "CLOCK_REALTIME not supported");
    result = static_cast<timestamp_t>(static_cast<int64_t>(1000000000UL) * static_cast<int64_t>(ts.tv_sec)
                                      + static_cast<int64_t>(ts.tv_nsec));
#else /* generic Unix */
    struct timeval tv;
#if JIMI_USE_ASSERT
    int status =
#endif /* JIMI_USE_ASSERT */
        gettimeofday(&tv, NULL);
    JIMI_ASSERT_EX(status == 0, "gettimeofday failed");
    result = static_cast<timestamp_t>(static_cast<int64_t>(1000000000UL) * static_cast<int64_t>(tv.tv_sec)
                                      + static_cast<int64_t>(1000UL) * static_cast<int64_t>(tv.tv_usec));
#endif /*(choice of OS) */

    return result;
}

/* 单位: 秒, second (浮点) */
template<class T>
inline double stop_watch_base<T>::internal_nowf(void)
{
    double result;

    timestamp_t time_usecs;
    time_usecs = stop_watch_base<T>::internal_now();

#if _WIN32 || _WIN64
    result = (double)time_usecs * 1E-9;
#elif __linux__
    result = (double)time_usecs * 1E-9;
#else  /* generic Unix */
    result = (double)time_usecs * 1E-6;
#endif  /*(choice of OS) */

    return result;
}

/* 单位: 毫秒, msec */
template<class T>
inline typename stop_watch_base<T>::timestamp_t
stop_watch_base<T>::internal_now_ms(void)
{
    timestamp_t result;

#if _WIN32 || _WIN64
    LARGE_INTEGER qp_cnt, qp_freq;
    QueryPerformanceCounter(&qp_cnt);
    QueryPerformanceFrequency(&qp_freq);
    result = static_cast<timestamp_t>(((double)qp_cnt.QuadPart / (double)qp_freq.QuadPart) * 1000.0);
#elif __linux__
    struct timespec ts;
#if JIMI_USE_ASSERT
    int status =
#endif /* JIMI_USE_ASSERT */
        clock_gettime(CLOCK_REALTIME, &ts);
    JIMI_ASSERT_EX(status == 0, "CLOCK_REALTIME not supported");
    result = static_cast<timestamp_t>(static_cast<int64_t>(1000UL) * static_cast<int64_t>(ts.tv_sec)
                                      + static_cast<int64_t>(ts.tv_nsec) / static_cast<int64_t>(1000000UL));
#else /* generic Unix */
    struct timeval tv;
#if JIMI_USE_ASSERT
    int status =
#endif /* JIMI_USE_ASSERT */
        gettimeofday(&tv, NULL);
    JIMI_ASSERT_EX(status == 0, "gettimeofday failed");
    result = static_cast<timestamp_t>(static_cast<int64_t>(1000UL) * static_cast<int64_t>(tv.tv_sec)
                                      + static_cast<int64_t>(tv.tv_usec) / static_cast<int64_t>(1000UL));
#endif /*(choice of OS) */

    return result;
}

template<class T>
inline void stop_watch_base<T>::internal_start()
{
    internal_getTimeStamp(startTime);
    stopTime = startTime;
}

template<class T>
inline void stop_watch_base<T>::internal_stop()
{
    internal_getTimeStamp(stopTime);
}

template<class T>
inline bool stop_watch_base<T>::isRunning(void)
{
    T *pT = static_cast<T *>(this);
    return pT->isRunning();
}

template<class T>
inline void stop_watch_base<T>::reset(void)
{
    T *pT = static_cast<T *>(this);
    pT->reset();
}

//! restart() is equivalent to reset() and begin()
template<class T>
inline void stop_watch_base<T>::restart(void)
{
    T *pT = static_cast<T *>(this);
    pT->restart();
}

template<class T>
inline void stop_watch_base<T>::rebegin(void)
{
    T *pT = static_cast<T *>(this);
    pT->restart();
}

template<class T>
inline void stop_watch_base<T>::start(void)
{
    T *pT = static_cast<T *>(this);
    pT->start();
}

template<class T>
inline void stop_watch_base<T>::begin(void)
{
    T *pT = static_cast<T *>(this);
    pT->start();
}

template<class T>
inline void stop_watch_base<T>::stop(void)
{
    T *pT = static_cast<T *>(this);
    pT->stop();
}

template<class T>
inline void stop_watch_base<T>::end(void)
{
    T *pT = static_cast<T *>(this);
    pT->stop();
}

template<class T>
inline typename stop_watch_base<T>::timestamp_t
stop_watch_base<T>::now(void)
{
    return stop_watch_base<T>::internal_now();
}

template<class T>
inline double stop_watch_base<T>::nowf(void)
{
    return stop_watch_base<T>::internal_nowf();
}

template<class T>
inline typename stop_watch_base<T>::timestamp_t
stop_watch_base<T>::now_ms(void)
{
    return stop_watch_base<T>::internal_now_ms();
}

template<class T>
inline double stop_watch_base<T>::intervalSeconds(int64_t t1, int64_t t2)
{
    double seconds = (double)(t2 - t1) * 1E-9;
    return seconds;
}

template<class T>
inline double stop_watch_base<T>::intervalSeconds(double t1, double t2)
{
    double seconds = (double)(t2 - t1);
    return seconds;
}

template<class T>
inline typename stop_watch_base<T>::timestamp_t
stop_watch_base<T>::getTimeStamp(void)
{
    timestamp_t timeStamp;
    internal_getTimeStamp(timeStamp);
    return timeStamp;
}

template<class T>
inline typename stop_watch_base<T>::timestamp_t
stop_watch_base<T>::currentTimeMillis(void)
{
    timestamp_t now_ms = stop_watch_base<T>::internal_now_ms();
    return now_ms;
}

template<class T>
inline double stop_watch_base<T>::currentTimeMillisf(void)
{
    timestamp_t now_usecs = stop_watch_base<T>::internal_now();
    return (double)now_usecs * 1E-6;
}

template<class T>
inline void stop_watch_base<T>::internal_getElapsedTime(timestamp_t &elapsedTime)
{
    T *pT = static_cast<T *>(this);
    return pT->internal_getElapsedTime(elapsedTime);
}

template<class T>
inline double stop_watch_base<T>::getMillisec(void)
{
    timestamp_t elapsedTime;
    T *pT = static_cast<T *>(this);
    pT->internal_getElapsedTime(elapsedTime);

#if _WIN32 || _WIN64
    LARGE_INTEGER qp_freq;
    QueryPerformanceFrequency(&qp_freq);
    return ((double)elapsedTime / (double)qp_freq.QuadPart) * 1E3;
#elif __linux__
    return (double)elapsedTime * 1E-6;
#else /* generic Unix */
    return (double)elapsedTime * 1E-3;
#endif /* (choice of OS) */
}

template<class T>
inline double stop_watch_base<T>::getSeconds(void)
{
    timestamp_t elapsedTime;
    T *pT = static_cast<T *>(this);
    pT->internal_getElapsedTime(elapsedTime);

#if _WIN32 || _WIN64
    LARGE_INTEGER qp_freq;
    QueryPerformanceFrequency(&qp_freq);
    return (double)elapsedTime / (double)qp_freq.QuadPart;
#elif __linux__
    return (double)elapsedTime * 1E-9;
#else /* generic Unix */
    return (double)elapsedTime * 1E-6;
#endif /* (choice of OS) */
}

template<class T>
inline double stop_watch_base<T>::getElapsedTime(void)
{
    return getSeconds();
}

///////////////////////////////////////////////////////////////////////////
// stop_watch
///////////////////////////////////////////////////////////////////////////

//! Absolute timestamp
/** @ingroup timing */
class stop_watch : public stop_watch_base<stop_watch>
{
public:
    //! Construct an absolute timestamp initialized to zero.
    stop_watch() : stop_watch_base<stop_watch>(), bIsRunning(false) { };
    stop_watch(const stop_watch &src);

    stop_watch &operator = (const stop_watch &sw);

    bool    isRunning(void) { return bIsRunning; };

    void    reset(void);

    //! restart() is equivalent to reset() and begin()
    void    restart(void);
    void    rebegin(void);

    void    start(void);
    void    stop(void);

    void    begin(void);
    void    end(void);

    void    internal_getElapsedTime(timestamp_t &elapsedTime);

protected:
    bool    bIsRunning;
};

inline stop_watch::stop_watch(const stop_watch &src) : stop_watch_base<stop_watch>(src)
{
    bIsRunning  = src.bIsRunning;
}

inline stop_watch &stop_watch::operator = (const stop_watch &sw)
{
    startTime   = sw.startTime;
    stopTime    = sw.stopTime;
    bIsRunning  = sw.bIsRunning;
    return *this;
}

inline void stop_watch::reset(void)
{
    startTime   = 0;
    stopTime    = 0;

    bIsRunning  = false;
}

//! restart() is equivalent to reset() and begin()
inline void stop_watch::restart(void)
{
    internal_start();
    bIsRunning = true;
}

inline void stop_watch::rebegin(void)
{
    restart();
}

inline void stop_watch::start(void)
{
    if (!bIsRunning) {
        internal_start();
        bIsRunning = true;
    }
}

inline void stop_watch::begin(void)
{
    start();
}

inline void stop_watch::stop(void)
{
    if (bIsRunning) {
        internal_stop();
        bIsRunning = false;
    }
}

inline void stop_watch::end(void)
{
    stop();
}

inline void stop_watch::internal_getElapsedTime(timestamp_t &elapsedTime)
{
    if (bIsRunning) {
        timestamp_t nowTime_;
        internal_getTimeStamp(nowTime_);
        elapsedTime = nowTime_ - startTime;
    }
    else {
        elapsedTime = stopTime - startTime;
    }
}

///////////////////////////////////////////////////////////////////////////
// stop_watch_ex
///////////////////////////////////////////////////////////////////////////

//! Absolute timestamp
/** @ingroup timing */
class stop_watch_ex : public stop_watch_base<stop_watch_ex>
{
public:
    //! Construct an absolute timestamp initialized to zero.
    stop_watch_ex() : stop_watch_base<stop_watch_ex>(),
        suspendStartTime(0), suspendTotalTime(0), elapsedTimeTotal(0),
        bIsRunning(false), bIsSuspended(false) { };
    stop_watch_ex(const stop_watch_ex &src);

    stop_watch_ex &operator = (const stop_watch_ex &sw);

    bool    isRunning(void)   { return bIsRunning;   };
    bool    isSuspended(void) { return bIsSuspended; };

    void    clear(void);
    void    reset(void);

    //! restart() is equivalent to reset() and begin()
    void    restart(void);
    void    rebegin(void);

    void    start(void);
    void    stop(void);

    void    begin(void);
    void    end(void);

    //! for suspend and resume
    void    suspend(void);
    void    resume(void);

    void    pause(void);
    void    continue_(void);

    void    internal_getElapsedTime(timestamp_t &elapsedTime);
    void    internal_getElapsedTimeTotal(timestamp_t &elapsedTime);

    //! get total times
    double  getTotalMillisec(void);
    double  getTotalSeconds(void);
    double  getTotalElapsedTime(void);

protected:
    //! for suspend and resume
    void    internal_suspend_start();
    void    internal_suspend_stop();

protected:
    timestamp_t suspendStartTime;
    timestamp_t suspendTotalTime;
    timestamp_t elapsedTimeTotal;
    bool        bIsRunning, bIsSuspended;
};

inline stop_watch_ex::stop_watch_ex(const stop_watch_ex &src)
    : stop_watch_base<stop_watch_ex>(src)
{
    suspendStartTime    = src.suspendStartTime;
    suspendTotalTime    = src.suspendTotalTime;
    elapsedTimeTotal    = src.elapsedTimeTotal;
    bIsRunning          = src.bIsRunning;
    bIsSuspended        = src.bIsSuspended;
}

inline stop_watch_ex &stop_watch_ex::operator = (const stop_watch_ex &sw)
{
    startTime           = sw.startTime;
    stopTime            = sw.stopTime;

    suspendStartTime    = sw.suspendStartTime;
    suspendTotalTime    = sw.suspendTotalTime;
    elapsedTimeTotal    = sw.elapsedTimeTotal;
    bIsRunning          = sw.bIsRunning;
    bIsSuspended        = sw.bIsSuspended;
    return *this;
}

inline void stop_watch_ex::internal_suspend_start()
{
    internal_getTimeStamp(suspendStartTime);
}

inline void stop_watch_ex::internal_suspend_stop()
{
    timestamp_t suspendStopTime_;
    internal_getTimeStamp(suspendStopTime_);

    suspendTotalTime += suspendStopTime_ - suspendStartTime;
}

inline void stop_watch_ex::clear(void)
{
    startTime           = 0;
    stopTime            = 0;
    suspendStartTime    = 0;
    suspendTotalTime    = 0;

    elapsedTimeTotal    = 0;

    bIsRunning   = false;
    bIsSuspended = false;
}

inline void stop_watch_ex::reset(void)
{
    startTime           = 0;
    stopTime            = 0;
    suspendStartTime    = 0;
    suspendTotalTime    = 0;

    bIsRunning   = false;
    bIsSuspended = false;
}

//! restart() is equivalent to reset() and begin()
inline void stop_watch_ex::restart(void)
{
    suspendStartTime    = 0;
    suspendTotalTime    = 0;

    internal_start();

    bIsRunning   = true;
    bIsSuspended = false;
}

inline void stop_watch_ex::rebegin(void)
{
    restart();
}

inline void stop_watch_ex::start(void)
{
    if (!bIsRunning) {
        internal_start();
        bIsRunning   = true;
        bIsSuspended = false;
    }
}

inline void stop_watch_ex::begin(void)
{
    start();
}

inline void stop_watch_ex::stop(void)
{
    if (bIsRunning) {
        timestamp_t elapsedTime;
        internal_stop();
        if (!bIsSuspended)
            elapsedTime = stopTime - startTime - suspendTotalTime;
        else
            elapsedTime = suspendStartTime - startTime;

        if (elapsedTime > (timestamp_t)0)
            elapsedTimeTotal += elapsedTime;

        bIsRunning   = false;
        bIsSuspended = false;
    }
}

inline void stop_watch_ex::end(void)
{
    stop();
}

inline void stop_watch_ex::suspend(void)
{
    if (!bIsSuspended) {
        internal_suspend_start();
        bIsSuspended = true;
    }
}

inline void stop_watch_ex::resume(void)
{
    if (bIsSuspended) {
        internal_suspend_stop();
        bIsSuspended = false;
    }
}

inline void stop_watch_ex::pause(void)
{
    suspend();
}

inline void stop_watch_ex::continue_(void)
{
    resume();
}

inline void stop_watch_ex::internal_getElapsedTime(timestamp_t &elapsedTime)
{
    if (bIsRunning) {
        timestamp_t nowTime_;
        internal_getTimeStamp(nowTime_);
        if (!bIsSuspended)
            elapsedTime = nowTime_ - startTime - suspendTotalTime;
        else
            elapsedTime = suspendStartTime - startTime;
    }
    else {
        if (!bIsSuspended)
            elapsedTime = stopTime - startTime - suspendTotalTime;
        else
            elapsedTime = suspendStartTime - startTime;
    }
}

inline void stop_watch_ex::internal_getElapsedTimeTotal(timestamp_t &elapsedTimeTotal_)
{
    if (bIsRunning) {
        timestamp_t elapsedTime;
        internal_getElapsedTime(elapsedTime);
        if (elapsedTime > (timestamp_t)0)
            elapsedTimeTotal_ = elapsedTimeTotal + elapsedTime;
        else
            elapsedTimeTotal_ = elapsedTimeTotal;
    }
    else {
        elapsedTimeTotal_ = elapsedTimeTotal;
    }
}

inline double stop_watch_ex::getTotalMillisec(void)
{
    timestamp_t elapsedTimeTotal_;
    internal_getElapsedTimeTotal(elapsedTimeTotal_);

#if _WIN32 || _WIN64
    LARGE_INTEGER qp_freq;
    QueryPerformanceFrequency(&qp_freq);
    return ((double)elapsedTimeTotal_ / (double)qp_freq.QuadPart) * 1E3;
#elif __linux__
    return (double)elapsedTimeTotal_ * 1E-6;
#else /* generic Unix */
    return (double)elapsedTimeTotal_ * 1E-3;
#endif /* (choice of OS) */
}

inline double stop_watch_ex::getTotalSeconds(void)
{
    timestamp_t elapsedTimeTotal_;
    internal_getElapsedTimeTotal(elapsedTimeTotal_);

#if _WIN32 || _WIN64
    LARGE_INTEGER qp_freq;
    QueryPerformanceFrequency(&qp_freq);
    return (double)elapsedTimeTotal_ / (double)qp_freq.QuadPart;
#elif __linux__
    return (double)elapsedTimeTotal_ * 1E-9;
#else /* generic Unix */
    return (double)elapsedTimeTotal_ * 1E-6;
#endif /* (choice of OS) */
}

inline double stop_watch_ex::getTotalElapsedTime(void)
{
    return getTotalSeconds();
}

}  /* namespace jimi */

#endif  /* _JIMI_STOP_WATCH_H_ */
