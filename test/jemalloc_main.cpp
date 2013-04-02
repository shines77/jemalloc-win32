
#include <vld.h>

#include <assert.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <errno.h>

#include "jemalloc_test.h"
#include "memory_pool_test.h"
#include "orgi_malloc_test.h"
#include "je_malloc_test.h"
#include "apr_malloc_test.h"

#define FIXED_RANDOM_SEED       31415926UL
#define MAX_LOOP_COUNT          1000000

mpool_test_data_t orig_malloc_test_params[] = {
    // define fixed size and repeated alloca
    ST_FIXED_REPEATED,  ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    // fixed size and repeated alloca
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8,          8,          1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16,         16,         1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      32,         32,         1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      64,         64,         1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      128,        128,        1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      256,        256,        1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      512,        512,        1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      1024,       1024,       1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      2048,       2048,       1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      4096,       4096,       1000000,    0,      0,

    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8192,       8192,       1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16384,      16384,      1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      32768,      32768,      1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      65536,      65536,      1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      131072,     131072,     1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      262144,     262144,     1000000,    0,      0,

#ifndef _DEBUG
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      524288,     524288,     100000,     0,      0,

    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      1048576,    1048576,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      2097152,    2097152,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      4194304,    4194304,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8388608,    8388608,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16777216,   16777216,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      33554432,   33554432,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      67108864,   67108864,   100000,     0,      0,
#else
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      524288,     524288,     100000,     0,      0,

    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      1048576,    1048576,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      2097152,    2097152,    50000,      0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      4194304,    4194304,    50000,      0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8388608,    8388608,    25000,      0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16777216,   16777216,   10000,      0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      33554432,   33554432,   10000,      0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      67108864,   67108864,   10000,      0,      0,
#endif

    // separator
    //ST_SEPARATOR,       ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    // define continuous size and repeated alloca
    ST_CONTI_REPEATED,  ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    // continuous size and repeated alloca
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          256,        1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          512,        1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          1024,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          2048,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          4096,       1000000,    0,      0,

    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          8192,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          16384,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          32768,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          65536,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          131072,     1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          262144,     1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          524288,     1000000,    0,      0,

    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          1048576,    100000,     0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          2097152,    100000,     0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          4194304,    100000,     0,      0,

    // define random size and repeated alloca
    ST_RANDOM_REPEATED, ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          256,        1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          512,        1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          1024,       1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          2048,       1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          4096,       1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          8192,       1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          16384,      1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          32768,      1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          65536,      1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          131072,     1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          262144,     1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          524288,     1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          1048576,    100000,     0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          2097152,    100000,     0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          4194304,    100000,     0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          8388608,    100000,     0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          16777216,   100000,     0,      0,

    // separator
    //ST_SEPARATOR,       ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,
};

mpool_test_data_t je_malloc_test_params[] = {
    // define fixed size and repeated alloca
    ST_FIXED_REPEATED,  ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    // fixed size and repeated alloca
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8,          8,          1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16,         16,         1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      32,         32,         1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      64,         64,         1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      128,        128,        1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      256,        256,        1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      512,        512,        1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      1024,       1024,       1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      2048,       2048,       1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      4096,       4096,       1000000,    0,      0,

    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8192,       8192,       1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16384,      16384,      1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      32768,      32768,      1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      65536,      65536,      1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      131072,     131072,     1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      262144,     262144,     1000000,    0,      0,

#ifndef _DEBUG
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      524288,     524288,     100000,     0,      0,

    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      1048576,    1048576,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      2097152,    2097152,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      4194304,    4194304,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8388608,    8388608,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16777216,   16777216,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      33554432,   33554432,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      67108864,   67108864,   100000,     0,      0,
#else
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      524288,     524288,     100000,     0,      0,

    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      1048576,    1048576,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      2097152,    2097152,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      4194304,    4194304,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8388608,    8388608,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16777216,   16777216,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      33554432,   33554432,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      67108864,   67108864,   100000,     0,      0,
#endif

    // separator
    //ST_SEPARATOR,       ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    // define continuous size and repeated alloca
    ST_CONTI_REPEATED,  ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    // continuous size and repeated alloca
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          256,        1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          512,        1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          1024,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          2048,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          4096,       1000000,    0,      0,

    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          8192,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          16384,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          32768,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          65536,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          131072,     1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          262144,     1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          524288,     1000000,    0,      0,

    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          1048576,    100000,     0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          2097152,    100000,     0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          4194304,    100000,     0,      0,

    // define random size and repeated alloca
    ST_RANDOM_REPEATED, ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          256,        1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          512,        1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          1024,       1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          2048,       1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          4096,       1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          8192,       1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          16384,      1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          32768,      1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          65536,      1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          131072,     1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          262144,     1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          524288,     1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          1048576,    100000,     0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          2097152,    100000,     0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          4194304,    100000,     0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          8388608,    100000,     0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          16777216,   100000,     0,      0,

    // separator
    //ST_SEPARATOR,       ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,
};

mpool_test_data_t apr_malloc_test_params[] = {
    // define fixed size and repeated alloca
    ST_FIXED_REPEATED,  ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    // fixed size and repeated alloca
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8,          8,          1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16,         16,         1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      32,         32,         1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      64,         64,         1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      128,        128,        1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      256,        256,        1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      512,        512,        1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      1024,       1024,       1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      2048,       2048,       1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      4096,       4096,       1000000,    0,      0,

    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8192,       8192,       1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16384,      16384,      1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      32768,      32768,      1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      65536,      65536,      1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      131072,     131072,     1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      262144,     262144,     1000000,    0,      0,

#ifndef _DEBUG
    //ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      524288,     524288,     100000,     0,      0,      // *
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      524288,     524288,     1000000,    0,      0,

    //ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      1048576,    1048576,    100000,     0,      0,      // *
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      1048576,    1048576,    1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      2097152,    2097152,    1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      4194304,    4194304,    1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8388608,    8388608,    1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16777216,   16777216,   1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      33554432,   33554432,   1000000,    0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      67108864,   67108864,   1000000,    0,      0,
#else
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      524288,     524288,     100000,     0,      0,

    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      1048576,    1048576,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      2097152,    2097152,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      4194304,    4194304,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8388608,    8388608,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16777216,   16777216,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      33554432,   33554432,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      67108864,   67108864,   100000,     0,      0,
#endif

    // separator
    //ST_SEPARATOR,       ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    // define continuous size and repeated alloca
    ST_CONTI_REPEATED,  ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    // continuous size and repeated alloca
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          256,        1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          512,        1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          1024,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          2048,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          4096,       1000000,    0,      0,

    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          8192,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          16384,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          32768,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          65536,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          131072,     1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          262144,     1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          524288,     1000000,    0,      0,

    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          1048576,    1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          2097152,    1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1,          4194304,    1000000,    0,      0,

    // define random size and repeated alloca
    ST_RANDOM_REPEATED, ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          256,        1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          512,        1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          1024,       1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          2048,       1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          4096,       1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          8192,       1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          16384,      1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          32768,      1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          65536,      1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          131072,     1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          262144,     1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          524288,     1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          1048576,    1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          2097152,    1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          4194304,    1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          8388608,    1000000,    0,      0,
    ST_RANDOM_SIZE,     AW_REPEATED_ALLOC,      1,          16777216,   1000000,    0,      0,

    // separator
    //ST_SEPARATOR,       ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,
};

char *format_alloc_size(char *buf, size_t size, int len)
{
    double fnum;
    size_t num;
    int i, rest_len;
    char fmt[64], unit[64];
    unit[0] = '\0';
    if (buf != NULL) {
        // if size is power of 2 or equal 0
        if ((size & (size - 1)) == 0) {
            if (size >= 1024) {
                if (size >= 1048576) {
                    num = size / 1048576;
                    strcpy(unit, "MB");
                }
                else {
                    num = size / 1024;
                    strcpy(unit, "KB");
                }
                sprintf(buf, "%3d %s", num, unit);
            }
            else {
                sprintf(fmt, "%%%dd", 3);
                sprintf(buf, fmt, size);
            }
        }
        else {
            if (size >= 1024) {
                if (size >= 1048576) {
                    fnum = (double)size / 1048576.0;
                    strcpy(unit, "MB");
                }
                else {
                    fnum = (double)size / 1024.0;
                    strcpy(unit, "KB");
                }
                sprintf(buf, "%03.1f %s", fnum, unit);
            }
            else {
                sprintf(fmt, "%%%dd", 3);
                sprintf(buf, fmt, size);
            }
        }
        rest_len = len - strlen(buf);
        if (rest_len > 0) {
            for (i = 0; i < rest_len; i++) {
                strcat(buf, " ");
            }
        }
        return buf;
    }
    else
        return "";
}

double original_malloc_test()
{
    int i;
    double used_time, total_time, fmultiple;
    char buf[128];
    int test_length = sizeof(orig_malloc_test_params) / sizeof(mpool_test_data_t);
    mpool_test_data_t *test_params = &orig_malloc_test_params[0];

    srand(FIXED_RANDOM_SEED);
    total_time = 0.0;

    mem_pool_test_linstener *tester = new orig_malloc_test();
        //new mem_pool_test   (ST_FIXED_SIZE, AW_REPEATED_ALLOC, 1,     16384, 100000);
        //new orig_malloc_test(ST_FIXED_SIZE, AW_REPEATED_ALLOC, 1,     16384, 100000);
        //new orig_malloc_test(ST_FIXED_SIZE, AW_REPEATED_ALLOC, 4096,  4096,  1000000);
    orig_malloc_test *tester_host = (orig_malloc_test *)tester;

    if (tester != NULL && tester_host != NULL) {
#if defined(_LANG_ID) && (_LANG_ID == 1)
        mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        mem_pool_printf("  内存分配函数: %s\n", tester->GetFuncName());
        mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
        for (i = 0; i < test_length; i++) {
            if (test_params[i].size_type == ST_SEPARATOR) {
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_FIXED_REPEATED) {
                mem_pool_printf("----------------------------------------------------------------\n");
                mem_pool_printf("  分配方式: 分配大小固定, 重复分配 (分配后立刻释放)\n");
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_CONTI_REPEATED) {
                mem_pool_printf("----------------------------------------------------------------\n");
                mem_pool_printf("  分配方式: 分配大小递增, 重复分配 (分配后立刻释放)\n");
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_RANDOM_REPEATED) {
                mem_pool_printf("----------------------------------------------------------------\n");
                mem_pool_printf("  分配方式: 分配大小随机, 重复分配 (分配后立刻释放)\n");
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            if (test_params[i].size_type == ST_RANDOM_SIZE) {
                // random seed
                srand(test_params[i].max_alloc_size);
            }
            // setting params
            fmultiple = (double)MAX_LOOP_COUNT / (double)test_params[i].loop_count1;
            tester_host->setting(test_params[i].size_type, test_params[i].alloc_way,
                test_params[i].min_alloc_size, test_params[i].max_alloc_size,
                test_params[i].loop_count1, test_params[i].loop_count2, test_params[i].loop_count3);
            tester->Begin();
            tester->RunTest();
            used_time = tester->End();
            total_time += used_time * fmultiple;
            if (test_params[i].size_type == ST_CONTINUOUS_SIZE
                || test_params[i].size_type == ST_RANDOM_SIZE) {
                //mem_pool_printf("分配大小: %d-%-8d    ", tester_host->min_alloc_size, tester_host->max_alloc_size);
                mem_pool_printf("分配大小: %d-%s    ", tester_host->min_alloc_size,
                    format_alloc_size(buf, tester_host->max_alloc_size, 8));
            }
            else {
                //mem_pool_printf("分配大小: %-8d    ", tester_host->min_alloc_size);
                mem_pool_printf("分配大小: %s    ", format_alloc_size(buf, tester_host->min_alloc_size, 8));
            }
            mem_pool_printf("重复次数: %-7d", (int)(tester_host->loop_count1 * fmultiple));
            if (test_params[i].loop_count1 < MAX_LOOP_COUNT)
                mem_pool_printf(" *  ");
            else
                mem_pool_printf("    ");
            mem_pool_printf("用时: %0.3f ms\n", used_time * fmultiple);
        }
        mem_pool_printf("----------------------------------------------------------------\n");
        mem_pool_printf("  %s    总用时: %0.3f 秒.\n", tester->GetFuncName(), total_time / 1000.0);
        mem_pool_printf("\n");
#else
        mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        mem_pool_printf("  Alloc Function: %s\n", tester->GetFuncName());
        mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
        for (i = 0; i < test_length; i++) {
            if (test_params[i].size_type == ST_SEPARATOR) {
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_FIXED_REPEATED) {
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                mem_pool_printf("  Alloc Mode: alloc size is fixed,\n
                mem_pool_printf("              repeat alloc (malloc and immediately free)\n");
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_CONTI_REPEATED) {
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                mem_pool_printf("  Alloc Mode: alloc size is continuously increase,\n
                mem_pool_printf("              repeat alloc (malloc and immediately free)\n");
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_RANDOM_REPEATED) {
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                mem_pool_printf("  Alloc Mode: alloc size is within a certain range of random numbers,\n
                mem_pool_printf("              repeat alloc (malloc and immediately free)\n");
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                continue;
            }
            if (test_params[i].size_type == ST_RANDOM_SIZE) {
                // random seed
                srand(test_params[i].max_alloc_size);
            }
            // setting params
            fmultiple = (double)MAX_LOOP_COUNT / (double)test_params[i].loop_count1;
            tester_host->setting(test_params[i].size_type, test_params[i].alloc_way,
                test_params[i].min_alloc_size, test_params[i].max_alloc_size,
                test_params[i].loop_count1, test_params[i].loop_count2, test_params[i].loop_count3);
            tester->Begin();
            tester->RunTest();
            used_time = tester->End();
            total_time += used_time * fmultiple;
            if (test_params[i].size_type == ST_CONTINUOUS_SIZE
                || test_params[i].size_type == ST_RANDOM_SIZE) {
                //mem_pool_printf("Alloc Size: %d-%-8d    ", tester_host->min_alloc_size, tester_host->max_alloc_size);
                mem_pool_printf("Alloc Size: %d-%s    ", tester_host->min_alloc_size,
                    format_alloc_size(buf, tester_host->max_alloc_size, 8));
            }
            else {
                //mem_pool_printf("Alloc Size: %-8d    ", tester_host->min_alloc_size);
                mem_pool_printf("Alloc Size: %s    ", format_alloc_size(buf, tester_host->min_alloc_size, 8));
            }
            mem_pool_printf("Repeat Times: %-7d", (int)(tester_host->loop_count1 * fmultiple));
            if (test_params[i].loop_count1 < MAX_LOOP_COUNT)
                mem_pool_printf(" *  ");
            else
                mem_pool_printf("    ");
            mem_pool_printf("Used Time: %0.3f ms\n", used_time * fmultiple);
        }
        mem_pool_printf("----------------------------------------------------------------\n");
        mem_pool_printf("  %s    Total Time: %0.3f seconds.\n", tester->GetFuncName(), total_time / 1000.0);
        mem_pool_printf("\n");
#endif
        if (tester != NULL) {
            delete tester;
            tester = NULL;
        }
        tester_host = NULL;
    }
    return total_time;
}

double je_malloc_test_()
{
    int i;
    double used_time, total_time, fmultiple;
    char buf[128];
    int test_length = sizeof(je_malloc_test_params) / sizeof(mpool_test_data_t);
    mpool_test_data_t *test_params = &je_malloc_test_params[0];

    srand(FIXED_RANDOM_SEED);
    total_time = 0.0;

    mem_pool_test_linstener *tester = new je_malloc_test();
        //new mem_pool_test (ST_FIXED_SIZE, AW_REPEATED_ALLOC, 1,       16384, 100000);
        //new je_malloc_test(ST_FIXED_SIZE, AW_REPEATED_ALLOC, 1,       16384, 100000);
        //new je_malloc_test(ST_FIXED_SIZE, AW_REPEATED_ALLOC, 4096,    4096,  1000000);
    je_malloc_test *tester_host = (je_malloc_test *)tester;

    if (tester != NULL && tester_host != NULL) {
        // setting params
        tester_host->setting(test_params[1].size_type, test_params[1].alloc_way,
            test_params[1].min_alloc_size, test_params[1].max_alloc_size,
            test_params[1].loop_count1, test_params[1].loop_count2, test_params[1].loop_count3);
        tester->Begin();
        tester->RunTest();
        used_time = tester->End();
        total_time += used_time;
        total_time -= total_time;
#if defined(_LANG_ID) && (_LANG_ID == 1)
        mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        mem_pool_printf("  内存分配函数: %s\n", tester->GetFuncName());
        mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
        for (i = 0; i < test_length; i++) {
            if (test_params[i].size_type == ST_SEPARATOR) {
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_FIXED_REPEATED) {
                mem_pool_printf("----------------------------------------------------------------\n");
                mem_pool_printf("  分配方式: 分配大小固定, 重复分配 (分配后立刻释放)\n");
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_CONTI_REPEATED) {
                mem_pool_printf("----------------------------------------------------------------\n");
                mem_pool_printf("  分配方式: 分配大小递增, 重复分配 (分配后立刻释放)\n");
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_RANDOM_REPEATED) {
                mem_pool_printf("----------------------------------------------------------------\n");
                mem_pool_printf("  分配方式: 分配大小随机, 重复分配 (分配后立刻释放)\n");
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            if (test_params[i].size_type == ST_RANDOM_SIZE) {
                // random seed
                srand(test_params[i].max_alloc_size);
            }
            // setting params
            fmultiple = (double)MAX_LOOP_COUNT / (double)test_params[i].loop_count1;
            tester_host->setting(test_params[i].size_type, test_params[i].alloc_way,
                test_params[i].min_alloc_size, test_params[i].max_alloc_size,
                test_params[i].loop_count1, test_params[i].loop_count2, test_params[i].loop_count3);
            tester->Begin();
            tester->RunTest();
            used_time = tester->End();
            total_time += used_time * fmultiple;
            if (test_params[i].size_type == ST_CONTINUOUS_SIZE
                || test_params[i].size_type == ST_RANDOM_SIZE) {
                //mem_pool_printf("分配大小: %d-%-8d    ", tester_host->min_alloc_size, tester_host->max_alloc_size);
                mem_pool_printf("分配大小: %d-%s    ", tester_host->min_alloc_size,
                    format_alloc_size(buf, tester_host->max_alloc_size, 8));
            }
            else {
                //mem_pool_printf("分配大小: %-8d    ", tester_host->min_alloc_size);
                mem_pool_printf("分配大小: %s    ", format_alloc_size(buf, tester_host->min_alloc_size, 8));
            }
            mem_pool_printf("重复次数: %-7d", (int)(tester_host->loop_count1 * fmultiple));
            if (test_params[i].loop_count1 < MAX_LOOP_COUNT)
                mem_pool_printf(" *  ");
            else
                mem_pool_printf("    ");
            mem_pool_printf("用时: %0.3f ms\n", used_time * fmultiple);
        }
        mem_pool_printf("----------------------------------------------------------------\n");
        mem_pool_printf("  %s    总用时: %0.3f 秒.\n", tester->GetFuncName(), total_time / 1000.0);
        mem_pool_printf("\n");
#else
        mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        mem_pool_printf("  Alloc Function: %s\n", tester->GetFuncName());
        mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
        for (i = 0; i < test_length; i++) {
            if (test_params[i].size_type == ST_SEPARATOR) {
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_FIXED_REPEATED) {
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                mem_pool_printf("  Alloc Mode: alloc size is fixed,\n
                mem_pool_printf("              repeat alloc (malloc and immediately free)\n");
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_CONTI_REPEATED) {
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                mem_pool_printf("  Alloc Mode: alloc size is continuously increase,\n
                mem_pool_printf("              repeat alloc (malloc and immediately free)\n");
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_RANDOM_REPEATED) {
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                mem_pool_printf("  Alloc Mode: alloc size is within a certain range of random numbers,\n
                mem_pool_printf("              repeat alloc (malloc and immediately free)\n");
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                continue;
            }
            if (test_params[i].size_type == ST_RANDOM_SIZE) {
                // random seed
                srand(test_params[i].max_alloc_size);
            }
            // setting params
            fmultiple = (double)MAX_LOOP_COUNT / (double)test_params[i].loop_count1;
            tester_host->setting(test_params[i].size_type, test_params[i].alloc_way,
                test_params[i].min_alloc_size, test_params[i].max_alloc_size,
                test_params[i].loop_count1, test_params[i].loop_count2, test_params[i].loop_count3);
            tester->Begin();
            tester->RunTest();
            used_time = tester->End();
            total_time += used_time * fmultiple;
            if (test_params[i].size_type == ST_CONTINUOUS_SIZE
                 || test_params[i].size_type == ST_RANDOM_SIZE) {
                //mem_pool_printf("Alloc Size: %d-%-8d    ", tester_host->min_alloc_size, tester_host->max_alloc_size);
                mem_pool_printf("Alloc Size: %d-%s    ", tester_host->min_alloc_size,
                    format_alloc_size(buf, tester_host->max_alloc_size, 8));
            }
            else {
                //mem_pool_printf("Alloc Size: %-8d    ", tester_host->min_alloc_size);
                mem_pool_printf("Alloc Size: %s    ", format_alloc_size(buf, tester_host->min_alloc_size, 8));
            }
            mem_pool_printf("Repeat Times: %-7d", (int)(tester_host->loop_count1 * fmultiple));
            if (test_params[i].loop_count1 < MAX_LOOP_COUNT)
                mem_pool_printf(" *  ");
            else
                mem_pool_printf("    ");
            mem_pool_printf("Used Time: %0.3f ms\n", used_time * fmultiple);
        }
        mem_pool_printf("----------------------------------------------------------------\n");
        mem_pool_printf("  %s    Total Time: %0.3f seconds.\n", tester->GetFuncName(), total_time / 1000.0);
        mem_pool_printf("\n");
#endif
        if (tester != NULL) {
            delete tester;
            tester = NULL;
        }
        tester_host = NULL;
    }
    return total_time;
}

double apr_malloc_test_()
{
    int i;
    double used_time, total_time, fmultiple;
    char buf[128];
    int test_length = sizeof(apr_malloc_test_params) / sizeof(mpool_test_data_t);
    mpool_test_data_t *test_params = &apr_malloc_test_params[0];

    srand(FIXED_RANDOM_SEED);
    total_time = 0.0;

    mem_pool_test_linstener *tester = new apr_malloc_test();
        //new mem_pool_test  (ST_FIXED_SIZE, AW_REPEATED_ALLOC, 1,       16384, 100000);
        //new apr_malloc_test(ST_FIXED_SIZE, AW_REPEATED_ALLOC, 1,       16384, 100000);
        //new apr_malloc_test(ST_FIXED_SIZE, AW_REPEATED_ALLOC, 4096,    4096,  1000000);
    apr_malloc_test *tester_host = (apr_malloc_test *)tester;

    if (tester != NULL && tester_host != NULL) {
        // setting params
        tester_host->setting(test_params[1].size_type, test_params[1].alloc_way,
            test_params[1].min_alloc_size, test_params[1].max_alloc_size,
            test_params[1].loop_count1, test_params[1].loop_count2, test_params[1].loop_count3);
        tester->Begin();
        tester->RunTest();
        used_time = tester->End();
        total_time += used_time;
        total_time -= total_time;
#if defined(_LANG_ID) && (_LANG_ID == 1)
        mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        mem_pool_printf("  内存分配函数: %s\n", tester->GetFuncName());
        mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
        for (i = 0; i < test_length; i++) {
            if (test_params[i].size_type == ST_SEPARATOR) {
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_FIXED_REPEATED) {
                mem_pool_printf("----------------------------------------------------------------\n");
                mem_pool_printf("  分配方式: 分配大小固定, 重复分配 (分配后立刻释放)\n");
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_CONTI_REPEATED) {
                mem_pool_printf("----------------------------------------------------------------\n");
                mem_pool_printf("  分配方式: 分配大小递增, 重复分配 (分配后立刻释放)\n");
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_RANDOM_REPEATED) {
                mem_pool_printf("----------------------------------------------------------------\n");
                mem_pool_printf("  分配方式: 分配大小随机, 重复分配 (分配后立刻释放)\n");
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            if (test_params[i].size_type == ST_RANDOM_SIZE) {
                // random seed
                srand(test_params[i].max_alloc_size);
            }
            // setting params
            fmultiple = (double)MAX_LOOP_COUNT / (double)test_params[i].loop_count1;
            tester_host->setting(test_params[i].size_type, test_params[i].alloc_way,
                test_params[i].min_alloc_size, test_params[i].max_alloc_size,
                test_params[i].loop_count1, test_params[i].loop_count2, test_params[i].loop_count3);
            tester->Begin();
            tester->RunTest();
            used_time = tester->End();
            total_time += used_time * fmultiple;
            if (test_params[i].size_type == ST_CONTINUOUS_SIZE
                || test_params[i].size_type == ST_RANDOM_SIZE) {
                //mem_pool_printf("分配大小: %d-%-8d    ", tester_host->min_alloc_size, tester_host->max_alloc_size);
                mem_pool_printf("分配大小: %d-%s    ", tester_host->min_alloc_size,
                    format_alloc_size(buf, tester_host->max_alloc_size, 8));
            }
            else {
                //mem_pool_printf("分配大小: %-8d    ", tester_host->min_alloc_size);
                mem_pool_printf("分配大小: %s    ", format_alloc_size(buf, tester_host->min_alloc_size, 8));
            }
            mem_pool_printf("重复次数: %-7d", (int)(tester_host->loop_count1 * fmultiple));
            if (test_params[i].loop_count1 < MAX_LOOP_COUNT)
                mem_pool_printf(" *  ");
            else
                mem_pool_printf("    ");
            mem_pool_printf("用时: %0.3f ms\n", used_time * fmultiple);
        }
        mem_pool_printf("----------------------------------------------------------------\n");
        mem_pool_printf("  %s    总用时: %0.3f 秒.\n", tester->GetFuncName(), total_time / 1000.0);
        mem_pool_printf("\n");
#else
        mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        mem_pool_printf("  Alloc Function: %s\n", tester->GetFuncName());
        mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
        for (i = 0; i < test_length; i++) {
            if (test_params[i].size_type == ST_SEPARATOR) {
                mem_pool_printf("----------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_FIXED_REPEATED) {
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                mem_pool_printf("  Alloc Mode: alloc size is fixed,\n
                mem_pool_printf("              repeat alloc (malloc and immediately free)\n");
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_CONTI_REPEATED) {
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                mem_pool_printf("  Alloc Mode: alloc size is continuously increase,\n
                mem_pool_printf("              repeat alloc (malloc and immediately free)\n");
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                continue;
            }
            else if (test_params[i].size_type == ST_RANDOM_REPEATED) {
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                mem_pool_printf("  Alloc Mode: alloc size is within a certain range of random numbers,\n
                mem_pool_printf("              repeat alloc (malloc and immediately free)\n");
                mem_pool_printf("-------------------------------------------------------------------------------\n");
                continue;
            }
            if (test_params[i].size_type == ST_RANDOM_SIZE) {
                // random seed
                srand(test_params[i].max_alloc_size);
            }
            // setting params
            fmultiple = (double)MAX_LOOP_COUNT / (double)test_params[i].loop_count1;
            tester_host->setting(test_params[i].size_type, test_params[i].alloc_way,
                test_params[i].min_alloc_size, test_params[i].max_alloc_size,
                test_params[i].loop_count1, test_params[i].loop_count2, test_params[i].loop_count3);
            tester->Begin();
            tester->RunTest();
            used_time = tester->End();
            total_time += used_time * fmultiple;
            if (test_params[i].size_type == ST_CONTINUOUS_SIZE
                 || test_params[i].size_type == ST_RANDOM_SIZE) {
                //mem_pool_printf("Alloc Size: %d-%-8d    ", tester_host->min_alloc_size, tester_host->max_alloc_size);
                mem_pool_printf("Alloc Size: %d-%s    ", tester_host->min_alloc_size,
                    format_alloc_size(buf, tester_host->max_alloc_size, 8));
            }
            else {
                //mem_pool_printf("Alloc Size: %-8d    ", tester_host->min_alloc_size);
                mem_pool_printf("Alloc Size: %s    ", format_alloc_size(buf, tester_host->min_alloc_size, 8));
            }
            mem_pool_printf("Repeat Times: %-7d", (int)(tester_host->loop_count1 * fmultiple));
            if (test_params[i].loop_count1 < MAX_LOOP_COUNT)
                mem_pool_printf(" *  ");
            else
                mem_pool_printf("    ");
            mem_pool_printf("Used Time: %0.3f ms\n", used_time * fmultiple);
        }
        mem_pool_printf("----------------------------------------------------------------\n");
        mem_pool_printf("  %s    Total Time: %0.3f seconds.\n", tester->GetFuncName(), total_time / 1000.0);
        mem_pool_printf("\n");
#endif
        if (tester != NULL) {
            delete tester;
            tester = NULL;
        }
        tester_host = NULL;
    }
    return total_time;
}

void Memory_Pool_Test()
{
    double time_orgi_malloc, time_je_malloc, time_apr_malloc;
    // original malloc()
    time_orgi_malloc = original_malloc_test();

    // je_malloc()
    time_je_malloc = je_malloc_test_();

    mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
#if defined(_LANG_ID) && (_LANG_ID == 1)
    mem_pool_printf("je_malloc() 速度提升比例: %0.2f %%\n", time_orgi_malloc * 100.0 / time_je_malloc);
#else
    mem_pool_printf("je_malloc() speed up: %0.2f %%\n", time_orgi_malloc * 100.0 / time_je_malloc);
#endif
    mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    mem_pool_printf("\n");

    // apr_malloc()
    time_apr_malloc = apr_malloc_test_();

    mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
#if defined(_LANG_ID) && (_LANG_ID == 1)
    mem_pool_printf("apr_malloc() 速度提升比例: %0.2f %%\n", time_orgi_malloc * 100.0 / time_apr_malloc);
#else
    mem_pool_printf("apr_malloc() speed up: %0.2f %%\n", time_orgi_malloc * 100.0 / time_apr_malloc);
#endif
    mem_pool_printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    mem_pool_printf("\n");
}

void Memory_Pool_Test_C()
{
    int i;
#ifdef _DEBUG
    const int nMaxLoop  = 100000;
    const int nMaxAlloc = 16384;
#else
    const int nMaxLoop  = 1000000;
    const int nMaxAlloc = 16384;
    //const int nMaxAlloc = 10000;
#endif

    unsigned int dwUsedTime1, dwUsedTime2;
    unsigned int dwStartTickCount = GetTickCount();
    for (i = 0; i < nMaxLoop; i++) {
        char *p = (char *)malloc((i % nMaxAlloc) + 1);
        if (p)
            free(p);
    }

    dwUsedTime1 = GetTickCount() - dwStartTickCount;
    mem_pool_printf("Alloc Func : malloc()     Alloc Size: 1-%d\n", nMaxAlloc);
    mem_pool_printf("Alloc Count: %d\n", nMaxLoop);
    mem_pool_printf("Total Cost : %d ms.\n", dwUsedTime1);

    //system("pause");
    mem_pool_printf("\n");

    set_errno(0);
    je_init();

    dwStartTickCount = GetTickCount();
    for (i = 0; i < nMaxLoop; i++) {
        void *p = je_malloc((i % nMaxAlloc) + 1);
        if (p)
            je_free(p);
    }

    je_uninit();

    dwUsedTime2 = GetTickCount() - dwStartTickCount;
    mem_pool_printf("Alloc Func : je_malloc()  Alloc Size: 1-%d\n", nMaxAlloc);
    mem_pool_printf("Alloc Count: %d\n", nMaxLoop);
    mem_pool_printf("Total Cost : %d ms.\t  Speed up: %0.2f %%\n", dwUsedTime2, ((double)(dwUsedTime1 * 10000 / dwUsedTime2) / 100.0));

    //system("pause");
    //mem_pool_printf("\n");
}

/*
 *
 * 顺序大小, 顺序释放, 随机大小(分不同的块), 乱序释放
 * 关于分块: 1-4096, 4096-16384, 16K-64K, 16K-84K, 128K-1M, 1M-64M
 *
 */
int main(int argc, char *argv[])
{
    // random seed
    srand((unsigned int)time(NULL));

    mem_pool_log_init(DEFAULT_LOG_FILENAME);

    Memory_Pool_Test();

    do {

        mem_pool_printf("-----------------------------------------------------\n");
        Memory_Pool_Test_C();
        mem_pool_printf("-----------------------------------------------------\n");

    } while (0);

    mem_pool_printf("\n");

    mem_pool_log_close();

#ifdef _WIN32
    system("pause");
#endif
    return 0;
}
