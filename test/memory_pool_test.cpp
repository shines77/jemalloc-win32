
#include "memory_pool_test.h"

mem_pool_test_param::mem_pool_test_param()
{
    size_type       = SIZE_TYPES_NONE;
    alloc_way       = ALLOC_WAYS_NONE;
    chunk_type      = 0;
    min_alloc_size  = 0;
    max_alloc_size  = 0;
    loop_count1     = 0;
    loop_count2     = 0;
    loop_count3     = 0;
}

mem_pool_test_param::mem_pool_test_param( size_types size_type_, alloc_ways alloc_way_,
                                         int min_alloc_size_, int max_alloc_size_,
                                         int loop_count1_,
                                         int loop_count2_ /*= 0*/,
                                         int loop_count3_ /*= 0*/ )
    : size_type(size_type_)
    , alloc_way(alloc_way_)
    , min_alloc_size(min_alloc_size_)
    , max_alloc_size(max_alloc_size_)
    , loop_count1(loop_count1_)
    , loop_count2(loop_count2_)
    , loop_count3(loop_count3_)
{
#ifdef _DEBUG
    loop_count1 = MAX(1, loop_count1_ / 100);
    loop_count2 = MAX(0, loop_count2_ / 100);
    loop_count3 = MAX(0, loop_count3_ / 100);
#endif
}

mem_pool_test_param::~mem_pool_test_param()
{

}

void mem_pool_test_param::setting( size_types size_type_, alloc_ways alloc_way_,
                                int min_alloc_size_, int max_alloc_size_,
                                int loop_count1_,
                                int loop_count2_ /*= 0*/,
                                int loop_count3_ /*= 0*/ )
{
    size_type       = size_type_;
    alloc_way       = alloc_way_;
    chunk_type      = 0;
    min_alloc_size  = min_alloc_size_;
    max_alloc_size  = max_alloc_size_;
#ifdef _DEBUG
    loop_count1 = MAX(1, loop_count1_ / 100);
    loop_count2 = MAX(0, loop_count2_ / 100);
    loop_count3 = MAX(0, loop_count3_ / 100);
#else
    loop_count1     = loop_count1_;
    loop_count2     = loop_count2_;
    loop_count3     = loop_count3_;
#endif
}

mem_pool_test::mem_pool_test() : mem_pool_test_param()
{
    chunk_type = 1;
}

mem_pool_test::mem_pool_test( size_types size_type_, alloc_ways alloc_way_,
                             int min_alloc_size_, int max_alloc_size_,
                             int loop_count1_,
                             int loop_count2_ /*= 0*/,
                             int loop_count3_ /*= 0*/ )
    : mem_pool_test_param(size_type_, alloc_way_,
                          min_alloc_size_, max_alloc_size_,
                          loop_count1_, loop_count2_, loop_count3_)
{

}

mem_pool_test::~mem_pool_test()
{

}

void mem_pool_test::RunTest( void )
{
#ifdef _DEBUG
    const int nMaxLoop  = 100000;
    const int nMaxAlloc = 16384;
#else
    const int nMaxLoop  = 1000000;
    const int nMaxAlloc = 16384;
    //const int nMaxAlloc = 10000;
#endif

    int i, j = 0;
    size_t alloc_size;
    void *p;
    if (size_type == ST_FIXED_SIZE && alloc_way == AW_REPEATED_ALLOC) {
        alloc_size = MAX(1, min_alloc_size);
        for (i = 0; i < loop_count1; i++) {
            p = Malloc(min_alloc_size);
            if (p)
                Free(p);
        }
    }
    else if (size_type == ST_CONTINUOUS_SIZE && alloc_way == AW_REPEATED_ALLOC) {
#if 1
        for (i = 0; i < loop_count1; i++) {
            alloc_size = (i % max_alloc_size) + min_alloc_size;
            alloc_size = MAX(1, alloc_size);
            p = Malloc(alloc_size);
            if (p)
                Free(p);
        }
#else
        for (i = 0; i < nMaxLoop; i++) {
            alloc_size = (i % nMaxAlloc) + 1;
            p = Malloc(alloc_size);
            if (p)
                Free(p);
        }
#endif
    }
}
