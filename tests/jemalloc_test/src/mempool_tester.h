
#ifndef _JEMALLOC_MEMPOOL_TESTER_H_
#define _JEMALLOC_MEMPOOL_TESTER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jemalloc_test.h"

#include "msvc_compat/stdint.h"
#include "stop_watch.h"

#include <stdio.h>

using namespace jimi;

#ifndef USE_STOP_WATCH
#define USE_STOP_WATCH      1
#endif

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

FILE *mempool_log_init(const char *filename);
void  mempool_log_close(void);

int   mempool_printf(char *fmt, ...);

size_t get_rand32();

#ifdef __cplusplus
}
#endif  /* __cplusplus */

enum size_types
{
    SIZE_TYPES_NONE = 0,
    ST_SEPARATOR,               // separator

    ST_FIXED_SIZE,              // fixed size
    ST_CONTINUOUS_SIZE,         // continuous size
    ST_RANDOM_SIZE,             // random size

    ST_FIXED_REPEATED,          // fixed size and repeated alloca
    ST_CONTI_REPEATED,          // continuous size and repeated alloca
    ST_RANDOM_REPEATED,         // random size and repeated alloca

    ST_FIXED_CONTIGUOUS,        // AW_CONTIGUOUS_ALLOC
    ST_RANDOM_CONTIGUOUS,       // AW_CONTIGUOUS_ALLOC
};

enum alloc_ways
{
    ALLOC_WAYS_NONE = 0,
    AW_REPEATED_ALLOC,          // repeated alloca
    AW_CONTIGUOUS_ALLOC,        // contiguous alloca
    AW_RANDOM_FREE,             // contiguous alloca and random order to free
};

////////////////////////////////////////////////////
// class test_data_t
////////////////////////////////////////////////////

typedef struct test_data_s
{
    size_types      size_type;
    alloc_ways      alloc_way;
//  int             chunk_type;
    unsigned int    min_alloc_size;
    unsigned int    max_alloc_size;
    int             loop_count1;
    int             loop_count2;
    int             loop_count3;
} test_data_t;

FORCE_INLINE
size_t get_rand32()
{
#if defined(RAND_MAX) && (RAND_MAX == 0x7FFF)
    return ((rand() & 0x7FFF) | ((rand() & 0x7FFF) << 15) | ((rand() & 0x7FFF) << 30));
#else
    return (rand() & 0xFFFF) | (rand() & 0xFFFF) << 16);
#endif
}

////////////////////////////////////////////////////
// class mempool_tester<T>
////////////////////////////////////////////////////

template <class T>
class mempool_tester
{
public:
    mempool_tester();
    mempool_tester(size_types size_type, alloc_ways alloc_way,
        int min_alloc_size, int max_alloc_size,
        int loop_count1, int loop_count2 = 0, int loop_count3 = 0);
    ~mempool_tester();

public:
    void Setting(size_types size_type, alloc_ways alloc_way,
                 int min_alloc_size, int max_alloc_size,
                 int loop_count1,
                 int loop_count2 /* = 0 */,
                 int loop_count3 /* = 0 */);

    char *GetFuncName();

    void *Malloc(size_t size);
    void *Realloc(void *p, size_t new_size);
    void Free(void *p);
    void Clear(void *p);

    void Run();

    void Reset()    { _sw.reset();      }
    void ClearSW()  { _sw.clear();      }
    void Restart()  { _sw.restart();    }
    void Start()    { _sw.start();      }
    void Stop()     { _sw.stop();       }

    double GetUsedTime() {
        return _sw.getMillisec();
    }

public:
    test_data_t test_param;

protected:
    stop_watch _sw;
};

template <class T>
mempool_tester<T>::mempool_tester()
{
    test_param.size_type       = SIZE_TYPES_NONE;
    test_param.alloc_way       = ALLOC_WAYS_NONE;
//  test_param.chunk_type      = 0;
    test_param.min_alloc_size  = 0;
    test_param.max_alloc_size  = 0;
    test_param.loop_count1     = 0;
    test_param.loop_count2     = 0;
    test_param.loop_count3     = 0;
}

template <class T>
mempool_tester<T>::mempool_tester(size_types size_type, alloc_ways alloc_way,
                                         int min_alloc_size, int max_alloc_size,
                                         int loop_count1, int loop_count2 /* = 0 */,
                                         int loop_count3 /* = 0 */)
{
    test_param.size_type       = size_type;
    test_param.alloc_way       = alloc_way;
//  test_param.chunk_type      = 0;
    test_param.min_alloc_size  = min_alloc_size;
    test_param.max_alloc_size  = max_alloc_size;
    test_param.loop_count1     = loop_count1;
    test_param.loop_count2     = loop_count2;
    test_param.loop_count3     = loop_count3;
}

template <class T>
mempool_tester<T>::~mempool_tester()
{
    // Do nothing!
}

template <class T>
void mempool_tester<T>::Setting(size_types size_type, alloc_ways alloc_way,
                                     int min_alloc_size, int max_alloc_size,
                                     int loop_count1, int loop_count2 /* = 0 */,
                                     int loop_count3 /* = 0 */)
{
    test_param.size_type       = size_type;
    test_param.alloc_way       = alloc_way;
//  test_param.chunk_type      = 0;
    test_param.min_alloc_size  = min_alloc_size;
    test_param.max_alloc_size  = max_alloc_size;
    test_param.loop_count1     = loop_count1;
    test_param.loop_count2     = loop_count2;
    test_param.loop_count3     = loop_count3;
}

template <class T>
inline
char * mempool_tester<T>::GetFuncName()
{
    //printf("mempool_tester<T>::GetFuncName();\n");
    T *pThis = static_cast<T *>(this);
    if (pThis) {
        return pThis->GetFuncName();
    }
    else
        return NULL;
}

template <class T>
inline
void * mempool_tester<T>::Malloc(size_t size)
{
    //printf("mempool_tester<T>::Malloc();\n");
    T *pThis = static_cast<T *>(this);
    if (pThis) {
        return pThis->Malloc(size);
    }
    else
        return NULL;
}

template <class T>
inline
void * mempool_tester<T>::Realloc(void *p, size_t new_size)
{
    //printf("mempool_tester<T>::Realloc();\n");
    T *pThis = static_cast<T *>(this);
    if (pThis) {
        return pThis->Realloc(p, new_size);
    }
    else
        return NULL;
}

template <class T>
inline
void mempool_tester<T>::Free(void *p)
{
    //printf("mempool_tester<T>::Free();\n");
    T *pThis = static_cast<T *>(this);
    if (pThis) {
        pThis->Free(p);
    }
}

template <class T>
inline
void mempool_tester<T>::Clear(void *p)
{
    //printf("mempool_tester<T>::Clear();\n");
    T *pThis = static_cast<T *>(this);
    if (pThis) {
        pThis->Clear(p);
    }
}

template <class T>
inline
void mempool_tester<T>::Run()
{
    const int kMaxPtrList = 1024;

    int i, j = 0;
    int imax, jmax;
    int loop_count;
    size_t alloc_size;
    size_t max_alloc;
    void *p;

    //printf("mempool_tester<T>::Run();\n");

    T *pThis = static_cast<T *>(this);
    if (pThis == NULL)
        return;

    if (test_param.size_type == ST_FIXED_SIZE && test_param.alloc_way == AW_REPEATED_ALLOC) {
        alloc_size = MAX(1, test_param.min_alloc_size);
        loop_count = test_param.loop_count1;
        for (i = 0; i < loop_count; i++) {
            p = pThis->Malloc(alloc_size);
            if (p)
                pThis->Free(p);
        }
    }
    else if (test_param.size_type == ST_CONTINUOUS_SIZE && test_param.alloc_way == AW_REPEATED_ALLOC) {
        max_alloc = test_param.max_alloc_size - test_param.min_alloc_size + 1;
        loop_count = test_param.loop_count1;
        alloc_size = test_param.min_alloc_size;
        for (i = 0; i < loop_count; i++) {
            //alloc_size = (i % max_alloc) + test_param.min_alloc_size;
            p = pThis->Malloc(alloc_size);
            if (p)
                pThis->Free(p);
            alloc_size++;
            if (alloc_size >= max_alloc)
                alloc_size = test_param.min_alloc_size;
        }
    }
    else if (test_param.size_type == ST_RANDOM_SIZE && test_param.alloc_way == AW_REPEATED_ALLOC) {
        size_t rnd;
        max_alloc = test_param.max_alloc_size - test_param.min_alloc_size + 1;
        loop_count = test_param.loop_count1;
        for (i = 0; i < loop_count; i++) {
            rnd = get_rand32();
            alloc_size = (rnd % max_alloc) + test_param.min_alloc_size;
            p = pThis->Malloc(alloc_size);
            if (p)
                pThis->Free(p);
        }
    }
    else if (test_param.size_type == ST_FIXED_SIZE && test_param.alloc_way == AW_CONTIGUOUS_ALLOC) {
        void *mem_list[kMaxPtrList];
        alloc_size = MAX(1, test_param.min_alloc_size);
        loop_count = test_param.loop_count1;
        imax = (loop_count + kMaxPtrList - 1) / kMaxPtrList;
        jmax = kMaxPtrList;
        for (i = 0; i < imax; i++) {
            for (j = 0; j < jmax; j++) {
                p = pThis->Malloc(alloc_size);
                mem_list[j] = p;
            }
            for (j = 0; j < jmax; j++) {
                p = mem_list[j];
                if (p)
                    pThis->Free(p);
            }
        }
    }
    else if (test_param.size_type == ST_RANDOM_SIZE && test_param.alloc_way == AW_CONTIGUOUS_ALLOC) {
        void *mem_list[kMaxPtrList];
        size_t rnd;
        max_alloc = test_param.max_alloc_size - test_param.min_alloc_size + 1;
        loop_count = test_param.loop_count1;
        imax = (loop_count + kMaxPtrList - 1) / kMaxPtrList;
        jmax = kMaxPtrList;
        for (i = 0; i < imax; i++) {
            for (j = 0; j < jmax; j++) {
                rnd = get_rand32();
                alloc_size = (rnd % max_alloc) + test_param.min_alloc_size;
                p = pThis->Malloc(alloc_size);
                mem_list[j] = p;
            }
            for (j = 0; j < jmax; j++) {
                p = mem_list[j];
                if (p)
                    pThis->Free(p);
            }
        }
    }
    else {
        //pThis->Run();
    }
}

#endif  /* _JEMALLOC_MEMPOOL_TESTER_H_ */
