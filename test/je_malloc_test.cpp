
#include "je_malloc_test.h"
#include "jemalloc/jemalloc.h"
#include "jemalloc/internal/jemalloc_internal.h"

je_malloc_test::je_malloc_test() : mem_pool_test()
{
    je_init();
}

je_malloc_test::je_malloc_test( size_types size_type_, alloc_ways alloc_way_,
                                   int min_alloc_size_, int max_alloc_size_,
                                   int loop_count1_,
                                   int loop_count2_ /*= 0*/,
                                   int loop_count3_ /*= 0*/ )
    : mem_pool_test(size_type_, alloc_way_,
                    min_alloc_size_, max_alloc_size_,
                    loop_count1_, loop_count2_, loop_count3_)
{
    je_init();
}

je_malloc_test::~je_malloc_test()
{
    je_uninit();
}

char * je_malloc_test::GetFuncName( void )
{
#if defined(_LANG_ID) && (_LANG_ID == 1)
    return "je_malloc() º¯Êý";
#else
    return "je_malloc()";
#endif
}

FORCE_INLINE void * je_malloc_test::Malloc( size_t size )
{
    return je_malloc(size);
}

FORCE_INLINE void je_malloc_test::Free( void *p )
{
    je_free(p);
}

FORCE_INLINE void * je_malloc_test::Realloc( void *p, size_t new_size )
{
    return je_realloc(p, new_size);
}

FORCE_INLINE void je_malloc_test::Clear( void *p )
{
    // do nothing
}
