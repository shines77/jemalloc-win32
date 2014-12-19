
#ifndef _JEMALLOC_MALLOC_TESTER_H_
#define _JEMALLOC_MALLOC_TESTER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jemalloc_test.h"
#include "mempool_tester.h"
#include "lang_def.h"

#include "jemalloc/jemalloc.h"
#include "msvc_compat/stdint.h"

#include <stdio.h>

////////////////////////////////////////////////////
// class malloc_tester
////////////////////////////////////////////////////

class malloc_tester : public mempool_tester<malloc_tester>
{
public:
    malloc_tester();
    malloc_tester(size_types size_type, alloc_ways alloc_way,
        int min_alloc_size, int max_alloc_size,
        int loop_count1, int loop_count2 = 0, int loop_count3 = 0);
    ~malloc_tester();

public:
    char *GetFuncName(void) {
        //printf("malloc_tester<T>::GetFuncName();\n");
        //return "class malloc_tester()";
        if (g_lang_id == LANG_ZH_CN)
            return "ÏµÍ³ malloc() º¯Êý";
        else
            return "original malloc() Function";
    }

    void *Malloc(size_t size) {
        //printf("malloc_tester<T>::Malloc();\n");
        return ::malloc(size);
    }

    void *Realloc(void *p, size_t new_size) {
        //printf("malloc_tester<T>::Realloc();\n");
        return ::realloc(p, new_size);
    }

    void Free(void *p) {
        //printf("malloc_tester<T>::Clear();\n");
        ::free(p);
    }

    void Clear(void *p) {
        //printf("malloc_tester<T>::Clear();\n");
        // Do nothing!
    }

    void Run(void) {
        //printf("malloc_tester<T>::Run();\n");
        mempool_tester<malloc_tester> *pThis = static_cast<mempool_tester<malloc_tester> *>(this);
        if (pThis) {
            pThis->Run();
        }
    }
};

#endif  /* _JEMALLOC_MALLOC_TESTER_H_ */
