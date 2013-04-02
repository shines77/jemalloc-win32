
#include "orgi_malloc_test.h"

orig_malloc_test::orig_malloc_test() : mem_pool_test()
{

}

orig_malloc_test::orig_malloc_test( size_types size_type_, alloc_ways alloc_way_,
                                   int min_alloc_size_, int max_alloc_size_,
                                   int loop_count1_,
                                   int loop_count2_ /*= 0*/,
                                   int loop_count3_ /*= 0*/ )
    : mem_pool_test(size_type_, alloc_way_,
                    min_alloc_size_, max_alloc_size_,
                    loop_count1_, loop_count2_, loop_count3_)
{

}

orig_malloc_test::~orig_malloc_test()
{

}

char * orig_malloc_test::GetFuncName( void )
{
#if defined(_LANG_ID) && (_LANG_ID == 1)
    return "ÏµÍ³ malloc() º¯Êý";
#else
    return "original malloc()";
#endif
}

FORCE_INLINE void * orig_malloc_test::Malloc( size_t size )
{
    return ::malloc(size);
}

FORCE_INLINE void orig_malloc_test::Free( void *p )
{
    ::free(p);
}

FORCE_INLINE void * orig_malloc_test::Realloc( void *p, size_t new_size )
{
    return ::realloc(p, new_size);
}

FORCE_INLINE void orig_malloc_test::Clear( void *p )
{
    // do nothing
}
