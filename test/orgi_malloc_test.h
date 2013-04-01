
#ifndef _ORIG_MALLOC_TEST_H_
#define _ORIG_MALLOC_TEST_H_

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#include "memory_pool_test.h"

class orig_malloc_test : public mem_pool_test
{
public:
    orig_malloc_test();
    orig_malloc_test(size_types size_type_, alloc_ways alloc_way_,
        int min_alloc_size_, int max_alloc_size_,
        int loop_count1_, int loop_count2_ = 0, int loop_count3_ = 0);
    virtual ~orig_malloc_test();

public:
    virtual void *Malloc(size_t size);
    virtual void  Free(void *p);
    virtual void *Realloc(void *p, size_t new_size);
    virtual void  Clear(void *p);

    virtual char *GetFuncName(void);
};

#endif  /* !_ORIG_MALLOC_TEST_H_ */
