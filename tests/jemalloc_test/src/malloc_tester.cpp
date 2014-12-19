
#include "malloc_tester.h"

malloc_tester::malloc_tester() : mempool_tester<malloc_tester>()
{
}

malloc_tester::malloc_tester(size_types size_type, alloc_ways alloc_way,
                                 int min_alloc_size, int max_alloc_size,
                                 int loop_count1, int loop_count2 /* = 0 */,
                                 int loop_count3 /* = 0 */)
 : mempool_tester<malloc_tester>(size_type, alloc_way,
                                 min_alloc_size, max_alloc_size,
                                 loop_count1, loop_count2, loop_count3)
{
}

malloc_tester::~malloc_tester()
{
    // Do nothing!
}
