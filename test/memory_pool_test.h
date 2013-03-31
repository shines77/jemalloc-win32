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

using namespace gmtl;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef struct mpool_test_data_s
{
    char    func_name[64];
    char    alloc_way_name[64];
    char    free_way_name[64];
    char    chunk_type_name[64];
    int     alloc_way;
    int     free_way;
    int     chunk_type;
    int     min_alloc_size;
    int     max_alloc_size;
} mpool_test_data_t;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

/* mem_pool_test_linstener */
class mem_pool_test_linstener
{
public:
    virtual void *Malloc(size_t size) = 0;
    virtual void Free(void *p) = 0;
    virtual void *ReMalloc(void *p, size_t new_size) = 0;
    virtual void Clear(void *p) = 0;

    virtual void BeginU(void) = 0;
    virtual uint32_t EndU(void) = 0;
    virtual void Begin(void) = 0;
    virtual double End(void) = 0;

public:
    virtual ~mem_pool_test_linstener() {};
};

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

    uint32_t EndU(void) {
        itimer::interval _interval = _timer.end();
        uint32_t usedTime = (uint32_t)(_interval.m_secs() * 1000.0);
        return usedTime;
    }

    void Begin(void) {
        _timer.begin();
    }

    double End(void) {
        itimer::interval _interval = _timer.end();
        double usedTime = _interval.m_secs();
        return usedTime;
    }

private:
    itimer _timer;
};

class mem_pool_test : public mem_pool_test_linstener
{
public:
    mem_pool_test();
    virtual ~mem_pool_test();

public:
    virtual void *Malloc(size_t size) { return NULL; };
    virtual void Free(void *p) { };
    virtual void *ReMalloc(void *p, size_t new_size) { return NULL; };
    virtual void Clear(void *p) { };

    virtual void BeginU(void) {
        _timer.begin();
    }

    virtual uint32_t EndU(void) {
        itimer::interval _interval = _timer.end();
        uint32_t usedTime = (uint32_t)(_interval.m_secs() * 1000.0);
        return usedTime;
    }

    virtual void Begin(void) {
        _timer.begin();
    }

    virtual double End(void) {
        itimer::interval _interval = _timer.end();
        double usedTime = _interval.m_secs();
        return usedTime;
    }

private:
    itimer _timer;
};

#endif  /* !__MEMORY_POOL_TEST_H__ */
