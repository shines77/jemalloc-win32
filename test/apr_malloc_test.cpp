
#include "apr_malloc_test.h"

apr_malloc_test::apr_malloc_test()
    : mem_pool_test()
    , apr_pmain(NULL)
    , apr_pchild(NULL)
{
    apr_pool_initialize();

    apr_new_pool();
}

apr_malloc_test::apr_malloc_test( size_types size_type_, alloc_ways alloc_way_,
                                   int min_alloc_size_, int max_alloc_size_,
                                   int loop_count1_,
                                   int loop_count2_ /*= 0*/,
                                   int loop_count3_ /*= 0*/ )
    : mem_pool_test(size_type_, alloc_way_,
                    min_alloc_size_, max_alloc_size_,
                    loop_count1_, loop_count2_, loop_count3_)
    , apr_pmain(NULL)
    , apr_pchild(NULL)
{
    apr_pool_initialize();

    apr_new_pool();
}

apr_malloc_test::~apr_malloc_test()
{
    apr_delete_pool();

    apr_pool_terminate();
}

bool apr_malloc_test::apr_new_pool()
{
    bool ret = false;

    // release first
    apr_delete_pool();

    apr_status_t rv;
    rv = apr_pool_create(&apr_pmain, NULL);
    assert(rv == APR_SUCCESS);
    if (rv == APR_SUCCESS) {
        rv = apr_pool_create(&apr_pchild, apr_pmain);
        assert(rv == APR_SUCCESS);
        if (rv == APR_SUCCESS) {
            ret = true;
        }
    }
    return ret;
}

void apr_malloc_test::apr_delete_pool()
{
    if (apr_pchild) {
        apr_pool_destroy(apr_pchild);
        apr_pchild = NULL;
    }
    if (apr_pmain) {
        apr_pool_destroy(apr_pmain);
        apr_pmain = NULL;
    }
}

char * apr_malloc_test::GetFuncName( void )
{
#if defined(_LANG_ID) && (_LANG_ID == 1)
    return "apr_palloc() º¯Êý";
#else
    return "apr_palloc()";
#endif
}

FORCE_INLINE void * apr_malloc_test::Malloc( size_t size )
{
    assert(apr_pchild != NULL);
    if (apr_pchild != NULL)
        return apr_palloc(apr_pchild, size);
    else
        return NULL;
}

FORCE_INLINE void apr_malloc_test::Free( void *p )
{
    if (apr_pchild != NULL)
        apr_pool_clear(apr_pchild);
}

FORCE_INLINE void * apr_malloc_test::Realloc( void *p, size_t new_size )
{
    assert(apr_pchild != NULL);
    if (apr_pchild != NULL) {
        apr_pool_clear(apr_pchild);
        return apr_palloc(apr_pchild, new_size);
    }
    else
        return NULL;
}

FORCE_INLINE void apr_malloc_test::Clear( void *p )
{
    // do nothing
}
