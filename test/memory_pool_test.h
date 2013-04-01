#ifndef __MEMORY_POOL_TEST_H__
#define __MEMORY_POOL_TEST_H__

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#include "jemalloc/jemalloc.h"
#include "jemalloc/internal/jemalloc_internal.h"

#include "msvc_compat/stdint.h"
#include "itimer.h"
#include "tickcount.h"

#define _USE_ITIMER_    1

#define _LANG_ID        1

#define MAX(a, b)       (((a)>(b)) ? (a) : (b))
#define MIN(a, b)       (((a)<(b)) ? (a) : (b))

#define FORCE_INLINE    __inline
//#define FORCE_INLINE

using namespace gmtl;

/* mem_test_timer */
class mem_test_timer
{
public:
    mem_test_timer() {};
    virtual ~mem_test_timer() {};

public:
    void BeginU(void) {
        _timer.begin();
    }

    void Begin(void) {
        _timer.begin();
    }

#if defined(_USE_ITIMER_) && (_USE_ITIMER_ != 0)
    uint32_t EndU(void) {
        itimer::interval _interval = _timer.end();
        uint32_t usedTime = (uint32_t)(_interval.m_secs() * 1000.0);
        return usedTime;
    }

    double End(void) {
        itimer::interval _interval = _timer.end();
        double usedTime = _interval.m_secs();
        return usedTime;
    }
#else
    uint32_t EndU(void) {
        tickcount::interval _interval = _timer.end();
        uint32_t usedTime = (uint32_t)(_interval.m_secs() * 1000.0);
        return usedTime;
    }

    double End(void) {
        tickcount::interval _interval = _timer.end();
        double usedTime = _interval.m_secs();
        return usedTime;
    }
#endif

private:
#if defined(_USE_ITIMER_) && (_USE_ITIMER_ != 0)
    itimer _timer;
#else
    tickcount _timer;
#endif
};

typedef enum _size_types
{
    SIZE_TYPES_NONE = 0,
    ST_SEPARATOR,               // separator

    ST_FIXED_SIZE,              // fixed size
    ST_CONTINUOUS_SIZE,         // continuous size
    ST_RANDOM_SIZE,             // random size

    ST_FIXED_REPEATED,          // fixed size and repeated alloca
    ST_CONTI_REPEATED,          // continuous size and repeated alloca
} size_types;

typedef enum _alloc_ways
{
    ALLOC_WAYS_NONE = 0,
    AW_REPEATED_ALLOC,          // repeated alloca
    AW_CONTIGUOUS_ALLOC,        // contiguous alloca
    AW_RANDOM_FREE,             // contiguous alloca and random order to free
} alloc_ways;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef struct mpool_test_data_s
{
    size_types  size_type;
    alloc_ways  alloc_way;
//  int         chunk_type;
    int         min_alloc_size;
    int         max_alloc_size;
    int         loop_count1;
    int         loop_count2;
    int         loop_count3;
} mpool_test_data_t;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

class mem_pool_test_param
{
public:
    mem_pool_test_param();
    mem_pool_test_param(size_types size_type_, alloc_ways alloc_way_,
        int min_alloc_size_, int max_alloc_size_,
        int loop_count1_, int loop_count2_ = 0, int loop_count3_ = 0);
    virtual ~mem_pool_test_param();

    void setting(size_types size_type_, alloc_ways alloc_way_,
        int min_alloc_size_, int max_alloc_size_,
        int loop_count1_, int loop_count2_ = 0, int loop_count3_ = 0);

public:
    int     size_type;
    int     alloc_way;
    int     chunk_type;
    int     min_alloc_size;
    int     max_alloc_size;
    int     loop_count1;
    int     loop_count2;
    int     loop_count3;

private:
    char    func_name[64];
};

/* mem_pool_test_linstener */
class mem_pool_test_linstener
{
public:
    virtual FORCE_INLINE void  *Malloc(size_t size) = 0;
    virtual FORCE_INLINE void   Free(void *p) = 0;
    virtual FORCE_INLINE void  *Realloc(void *p, size_t new_size) = 0;
    virtual FORCE_INLINE void   Clear(void *p) = 0;

    virtual void        RunTest(void) = 0;
    virtual char       *GetFuncName(void) = 0;

    virtual void        BeginU(void) = 0;
    virtual uint32_t    EndU(void) = 0;
    virtual void        Begin(void) = 0;
    virtual double      End(void) = 0;

public:
    virtual ~mem_pool_test_linstener() {};
};

class mem_pool_test : public mem_pool_test_param
                    , public mem_pool_test_linstener
{
public:
    mem_pool_test();
    mem_pool_test(size_types size_type_, alloc_ways alloc_way_,
        int min_alloc_size_, int max_alloc_size_,
        int loop_count1_, int loop_count2_ = 0, int loop_count3_ = 0);
    virtual ~mem_pool_test();

public:
    virtual FORCE_INLINE void *Malloc(size_t size) { return NULL; };
    virtual FORCE_INLINE void  Free(void *p) { };
    virtual FORCE_INLINE void *Realloc(void *p, size_t new_size) { return NULL; };
    virtual FORCE_INLINE void  Clear(void *p) { };

    virtual void  RunTest(void);
    virtual char *GetFuncName(void) {
        return "class mem_pool_test";
    };

    virtual void BeginU(void) {
        _timer.begin();
    }

    virtual void Begin(void) {
        _timer.begin();
    }

#if defined(_USE_ITIMER_) && (_USE_ITIMER_ != 0)
    virtual uint32_t EndU(void) {
        itimer::interval _interval = _timer.end();
        uint32_t usedTime = (uint32_t)(_interval.m_secs() * 1000.0);
        return usedTime;
    }

    virtual double End(void) {
        itimer::interval _interval = _timer.end();
        double usedTime = _interval.m_secs();
        return usedTime;
    }
#else
    virtual uint32_t EndU(void) {
        tickcount::interval _interval = _timer.end();
        uint32_t usedTime = (uint32_t)(_interval.m_secs() * 1000.0);
        return usedTime;
    }

    virtual double End(void) {
        tickcount::interval _interval = _timer.end();
        double usedTime = _interval.m_secs();
        return usedTime;
    }
#endif

private:
#if defined(_USE_ITIMER_) && (_USE_ITIMER_ != 0)
    itimer _timer;
#else
    tickcount _timer;
#endif
};

#endif  /* !__MEMORY_POOL_TEST_H__ */
