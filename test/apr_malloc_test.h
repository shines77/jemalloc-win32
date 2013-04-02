
#ifndef _APR_MALLOC_TEST_H_
#define _APR_MALLOC_TEST_H_

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#include "memory_pool_test.h"
#include "apr.h"
#include "apr_general.h"
#include "apr_pools.h"
#include "apr_errno.h"
#include "apr_file_io.h"
#if APR_HAVE_UNISTD_H
#include <unistd.h>
#endif

class apr_malloc_test : public mem_pool_test
{
public:
    apr_malloc_test();
    apr_malloc_test(size_types size_type_, alloc_ways alloc_way_,
        int min_alloc_size_, int max_alloc_size_,
        int loop_count1_, int loop_count2_ = 0, int loop_count3_ = 0);
    virtual ~apr_malloc_test();

public:
    virtual void *Malloc(size_t size);
    virtual void  Free(void *p);
    virtual void *Realloc(void *p, size_t new_size);
    virtual void  Clear(void *p);

    virtual char *GetFuncName(void);

protected:
    bool apr_new_pool();
    void apr_delete_pool();

private:
    apr_pool_t *apr_pmain;
    apr_pool_t *apr_pchild;
};

#endif  /* !_APR_MALLOC_TEST_H_ */
