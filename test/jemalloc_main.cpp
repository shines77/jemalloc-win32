
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

mpool_test_data_t orig_malloc_test_params[] = {
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
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      524288,     524288,     100000,     0,      0,

    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      1048576,    1048576,    100000,     0,      0,
#ifndef _DEBUG
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      2097152,    2097152,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      4194304,    4194304,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8388608,    8388608,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16777216,   16777216,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      33554432,   33554432,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      67108864,   67108864,   100000,     0,      0,
#else
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      2097152,    2097152,    50000,      0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      4194304,    4194304,    50000,      0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8388608,    8388608,    25000,      0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16777216,   16777216,   10000,      0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      33554432,   33554432,   10000,      0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      67108864,   67108864,   10000,      0,      0,
#endif

    // separator
    ST_SEPARATOR,       ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    // continuous size and repeated alloca
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      256,        256,        1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      512,        512,        1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1024,       1024,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      2048,       2048,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      4096,       4096,       1000000,    0,      0,

    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      8192,       8192,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      16384,      16384,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      32768,      32768,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      65536,      65536,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      131072,     131072,     1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      262144,     262144,     1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      524288,     524288,     1000000,     0,      0,

    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1048576,    1048576,    1000000,     0,      0,

    // separator
    ST_SEPARATOR,       ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,
};

mpool_test_data_t je_malloc_test_params[] = {
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
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      524288,     524288,     100000,     0,      0,

    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      1048576,    1048576,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      2097152,    2097152,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      4194304,    4194304,    100000,     0,      0,
#ifndef _DEBUG
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8388608,    8388608,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16777216,   16777216,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      33554432,   33554432,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      67108864,   67108864,   100000,     0,      0,
#else
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      8388608,    8388608,    100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      16777216,   16777216,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      33554432,   33554432,   100000,     0,      0,
    ST_FIXED_SIZE,      AW_REPEATED_ALLOC,      67108864,   67108864,   100000,     0,      0,
#endif

    // separator
    ST_SEPARATOR,       ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,

    // continuous size and repeated alloca
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      256,        256,        1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      512,        512,        1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1024,       1024,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      2048,       2048,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      4096,       4096,       1000000,    0,      0,

    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      8192,       8192,       1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      16384,      16384,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      32768,      32768,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      65536,      65536,      1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      131072,     131072,     1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      262144,     262144,     1000000,    0,      0,
    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      524288,     524288,     1000000,     0,      0,

    ST_CONTINUOUS_SIZE, AW_REPEATED_ALLOC,      1048576,    1048576,    1000000,     0,      0,

    // separator
    ST_SEPARATOR,       ALLOC_WAYS_NONE,        0,          0,          0,          0,      0,
};

void Hello_World()
{
    printf("\n");
    printf("Hello world !\n");
    printf("\n");
}

void original_malloc_test()
{
    int i;
    double used_time;
    int test_length = sizeof(orig_malloc_test_params) / sizeof(mpool_test_data_t);
    mpool_test_data_t *test_params = &orig_malloc_test_params[0];

    mem_pool_test_linstener *tester = new orig_malloc_test();
        //new mem_pool_test(ST_FIXED_SIZE, AW_REPEATED_ALLOC,    1,     16384, 100000);
        //new orig_malloc_test(ST_FIXED_SIZE, AW_REPEATED_ALLOC, 1,     16384, 100000);
        //new orig_malloc_test(ST_FIXED_SIZE, AW_REPEATED_ALLOC, 4096,  4096,  1000000);
    orig_malloc_test *tester_host = (orig_malloc_test *)tester;

    if (tester != NULL && tester_host != NULL) {
#if defined(_LANG_ID) && (_LANG_ID == 1)
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        printf("  内存分配函数: %s\n", tester->GetFuncName());
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
        for (i = 0; i < test_length; i++) {
            if (test_params[i].size_type == ST_SEPARATOR) {
                printf("----------------------------------------------------------------\n");
                continue;
            }
            tester_host->setting(test_params[i].size_type, test_params[i].alloc_way,
                test_params[i].min_alloc_size, test_params[i].max_alloc_size,
                test_params[i].loop_count1, test_params[i].loop_count2, test_params[i].loop_count3);
            tester->Begin();
            tester->RunTest();
            used_time = tester->End();
            printf("分配大小: %-8d    ", tester_host->min_alloc_size);
            printf("重复次数: %-7d    ", tester_host->loop_count1);
            printf("用时: %0.3f ms\n", used_time);
        }
#else
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        printf("  Alloc Function: %s\n", tester->GetFuncName());
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
        for (i = 0; i < test_length; i++) {
            if (test_params[i].size_type == ST_SEPARATOR) {
                printf("----------------------------------------------------------------\n");
                continue;
            }
            tester_host->setting(test_params[i].size_type, test_params[i].alloc_way,
                test_params[i].min_alloc_size, test_params[i].max_alloc_size,
                test_params[i].loop_count1, test_params[i].loop_count2, test_params[i].loop_count3);
            tester->Begin();
            tester->RunTest();
            used_time = tester->End();
            printf("Alloc Size: %-8d    ", tester_host->min_alloc_size);
            printf("Repeat Times: %-7d    ", tester_host->loop_count1);
            printf("Used Time: %0.3f ms\n", used_time);
        }
#endif
        printf("\n");
        if (tester != NULL) {
            delete tester;
            tester = NULL;
        }
        tester_host = NULL;
    }
}

void je_malloc_test_()
{
    int i;
    double used_time;
    int test_length = sizeof(je_malloc_test_params) / sizeof(mpool_test_data_t);
    mpool_test_data_t *test_params = &je_malloc_test_params[0];

    mem_pool_test_linstener *tester = new je_malloc_test();
        //new mem_pool_test(ST_FIXED_SIZE, AW_REPEATED_ALLOC,  1,       16384, 100000);
        //new je_malloc_test(ST_FIXED_SIZE, AW_REPEATED_ALLOC, 1,       16384, 100000);
        //new je_malloc_test(ST_FIXED_SIZE, AW_REPEATED_ALLOC, 4096,    4096,  1000000);
    je_malloc_test *tester_host = (je_malloc_test *)tester;

    if (tester != NULL && tester_host != NULL) {
#if defined(_LANG_ID) && (_LANG_ID == 1)
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        printf("  内存分配函数: %s\n", tester->GetFuncName());
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
        for (i = 0; i < test_length; i++) {
            if (test_params[i].size_type == ST_SEPARATOR) {
                printf("----------------------------------------------------------------\n");
                continue;
            }
            tester_host->setting(test_params[i].size_type, test_params[i].alloc_way,
                test_params[i].min_alloc_size, test_params[i].max_alloc_size,
                test_params[i].loop_count1, test_params[i].loop_count2, test_params[i].loop_count3);
            tester->Begin();
            tester->RunTest();
            used_time = tester->End();
            printf("分配大小: %-8d    ", tester_host->min_alloc_size);
            printf("重复次数: %-7d    ", tester_host->loop_count1);
            printf("用时: %0.3f ms\n", used_time);
        }
#else
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        printf("  Alloc Function: %s\n", tester->GetFuncName());
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n\n");
        for (i = 0; i < test_length; i++) {
            if (test_params[i].size_type == ST_SEPARATOR) {
                printf("----------------------------------------------------------------\n");
                continue;
            }
            tester_host->setting(test_params[i].size_type, test_params[i].alloc_way,
                test_params[i].min_alloc_size, test_params[i].max_alloc_size,
                test_params[i].loop_count1, test_params[i].loop_count2, test_params[i].loop_count3);
            tester->Begin();
            tester->RunTest();
            used_time = tester->End();
            printf("Alloc Size: %-8d    ", tester_host->min_alloc_size);
            printf("Repeat Times: %-7d    ", tester_host->loop_count1);
            printf("Used Time: %0.3f ms\n", used_time);
        }
#endif
        printf("\n");
        if (tester != NULL) {
            delete tester;
            tester = NULL;
        }
        tester_host = NULL;
    }
}

void Memory_Pool_Test()
{
    // original malloc()
    original_malloc_test();

    // je_malloc()
    je_malloc_test_();
}

void MemoryPool_Test()
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
    printf("Alloc Func : malloc()     Alloc Size: 1-%d\n", nMaxAlloc);
    printf("Alloc Count: %d\n", nMaxLoop);
    printf("Total Cost : %d ms.\n", dwUsedTime1);

    //system("pause");
    printf("\n");

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
    printf("Alloc Func : je_malloc()  Alloc Size: 1-%d\n", nMaxAlloc);
    printf("Alloc Count: %d\n", nMaxLoop);
    printf("Total Cost : %d ms.\t  Speed up: %0.2f %%\n", dwUsedTime2, ((double)(dwUsedTime1 * 10000 / dwUsedTime2) / 100.0));

    //system("pause");
    //printf("\n");
}

/*
 *
 * 顺序大小, 顺序释放, 随机大小(分不同的块), 乱序释放
 * 关于分块: 1-4096, 4096-16384, 16K-64K, 16K-84K, 128K-1M, 1M-64M
 *
 */

int main(int argc, char *argv[])
{
    Memory_Pool_Test();

    do {
        //Hello_World();

        printf("-----------------------------------------------------\n");
        MemoryPool_Test();
        printf("-----------------------------------------------------\n");

    } while (0);

    printf("\n");

#ifdef _WIN32
    system("pause");
#endif
    return 0;
}
