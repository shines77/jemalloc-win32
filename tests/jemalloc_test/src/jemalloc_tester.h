
#ifndef _JEMALLOC_JEMALLOC_TESTER_H_
#define _JEMALLOC_JEMALLOC_TESTER_H_

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
// class jemalloc_tester
////////////////////////////////////////////////////

class jemalloc_tester : public mempool_tester<jemalloc_tester>
{
public:
    jemalloc_tester();
    jemalloc_tester(size_types size_type, alloc_ways alloc_way,
        int min_alloc_size, int max_alloc_size,
        int loop_count1, int loop_count2 = 0, int loop_count3 = 0);
    ~jemalloc_tester();

public:
    char *GetFuncName(void) {
        //printf("jemalloc_tester<T>::GetFuncName();\n");
        //return "class jemalloc_tester()";
        if (g_lang_id == LANG_ZH_CN)
            return "je_malloc() º¯Êý";
        else
            return "je_malloc() Function";
    }

    void *Malloc(size_t size) {
        //printf("jemalloc_tester<T>::Malloc();\n");
        return je_malloc(size);
    }

    void *Realloc(void *p, size_t new_size) {
        //printf("jemalloc_tester<T>::Realloc();\n");
        return je_realloc(p, new_size);
    }

    void Free(void *p) {
        //printf("jemalloc_tester<T>::Clear();\n");
        je_free(p);
    }

    void Clear(void *p) {
        //printf("jemalloc_tester<T>::Clear();\n");
        // Do nothing!
    }

    void Run(void) {
        //printf("jemalloc_tester<T>::Run();\n");
        mempool_tester<jemalloc_tester> *pThis = static_cast<mempool_tester<jemalloc_tester> *>(this);
        if (pThis) {
            pThis->Run();
        }
    }
};

#endif  /* _JEMALLOC_JEMALLOC_TESTER_H_ */
