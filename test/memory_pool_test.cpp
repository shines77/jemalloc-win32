
#include "memory_pool_test.h"

#include <windows.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>
#include <stdio.h>
#include <share.h>

/* for get_current_dir() on linux */
#ifndef _WIN32
#include <limits.h>
#include <unistd.h>
#endif

static FILE *log_file = NULL;
static char log_filename[MAX_LOG_FILENAME_LEN];

/************************************************************************
 *
 * C string replace function
 * Descript: return a replacement string,
 *      exhausted after use free () to release.
 *   If the call succeeds returns a pointer that point the string is
 *      replaced with the new, returns NULL if it fails
 *
 * C 字符串替换函数
 * 描述: 返回一个替换以后的字符串, 用完之后要用free()释放
 *       如果调用成功返回指向被替换后新字符串的指针, 如果失败返回NULL
 *
 ************************************************************************/
char *str_replace(const char *strbuf, const char *sstr, const char *dstr)
{
    char *new_str = NULL, *next;
    unsigned int len, str_len, src_len, dst_len;
    register unsigned int i = 0;
    char *dup_buf;

    if ((strbuf == NULL) || (sstr == NULL) || (dstr == NULL))
        return NULL;

    dup_buf = (char *)strbuf;
    src_len = strlen(sstr);
    while (strstr(dup_buf, sstr) != NULL){
        dup_buf = strstr(dup_buf, sstr) + src_len;
        i++;
    }

    if (i != 0) {
        dup_buf = (char *)strbuf;
        str_len = strlen(strbuf);
        dst_len = strlen(dstr);
        len = (str_len - (src_len - dst_len) * i + 1) * sizeof(char);
        new_str = (char *)malloc(len);
        if (new_str != NULL) {
            //new_str[0] = '\0';
            memset(new_str, 0, len);
            while ((next = strstr(dup_buf, sstr)) != NULL) {
                new_str = strncat(new_str, dup_buf, (next - dup_buf));
                new_str = strcat(new_str, dstr);
                dup_buf = next + src_len;
            }
            new_str = strcat(new_str, dup_buf);
            return new_str;
        }
    }
    else {
        str_len = strlen(strbuf);
        len = (str_len + 1) * sizeof(char);
        new_str = (char *)malloc(len);
        if (new_str != NULL) {
            memset(new_str, 0, len);
            new_str = strcpy(new_str, strbuf);
            return new_str;
        }
    }
    return new_str;
}

#ifndef PATH_MAX
#define PATH_MAX    1024
#endif

#ifndef MAX_APP_PATH
#ifdef _WIN32
  #define MAX_APP_PATH    MAX_PATH
#else
  #define MAX_APP_PATH    PATH_MAX
#endif
#endif

char *get_current_dir(char *cur_app_path, unsigned int buf_size)
{
#ifdef _WIN32
    if (!GetModuleFileNameA(NULL, cur_app_path, buf_size)) {
        printf("Cannot get application current path. (%d)\n", GetLastError());
        return NULL;
    }
#else
    int i;

    // Get absolute path of the current program, 获取当前程序绝对路径
    int cnt = readlink("/proc/self/exe", cur_app_path, buf_size - 1);
    if (cnt < 0 || cnt >= buf_size - 1) {
        printf("*** readlink() Error! ***\n");

        // Get absolute path of the current directory, 获取当前目录绝对路径
        if (NULL == realpath("./", cur_app_path)) {
            printf("*** realpath() Error! ***\n");
            return NULL;
        }
        strcat(cur_app_path, "/");
    }
    else {
        cur_app_path[cnt] = '\0';
    }
    // Get the absolute path of the current directory, that is to remove the process name
    // 获取当前目录绝对路径，即去掉进程名
    for (i = cnt; i >= 0; --i) {
        if (cur_app_path[i] == '/') {
            cur_app_path[i + 1] = '\0';
            break;
        }
    }
#endif
    return cur_app_path;
}

FILE *mem_pool_log_init(const char *filename)
{
    errno_t err = 0;
    int fd = 0;
    char cur_app_path[MAX_APP_PATH];
    if (filename != NULL) {
#ifdef _WIN32
        if (get_current_dir(cur_app_path, MAX_APP_PATH) != NULL) {
            strcpy_s(log_filename, MAX_LOG_FILENAME_LEN, cur_app_path);
            strcat_s(log_filename, MAX_LOG_FILENAME_LEN, "\\");
            strcat_s(log_filename, MAX_LOG_FILENAME_LEN, filename);
        }
        else
            strcpy_s(log_filename, MAX_LOG_FILENAME_LEN, filename);
#else
        if (get_current_dir(cur_app_path, MAX_APP_PATH) != NULL) {
            // cur_app_path already contains "/"
            strcpy(log_filename, cur_app_path);
            //strcat(log_filename, "/");
            strcat(log_filename, filename);
        }
        else
            strcpy(log_filename, filename);
#endif
        mem_pool_log_close();

        // _O_APPEND, _O_CREAT, _SH_DENYRW, _SH_DENYNO
#ifdef _MSC_VER
        err = _sopen_s((int *)&fd, filename, _O_CREAT|_O_TRUNC|_O_BINARY|_O_RDWR,
            _SH_DENYNO, _S_IREAD|_S_IWRITE);
        if (err == 0) {
            log_file = (FILE *)fd;
            return log_file;
        }
#else
        fd = open(filename, _O_CREAT|_O_TRUNC|_O_BINARY|_O_RDWR, _S_IREAD|_S_IWRITE);
        if (fd != -1) {
            log_file = (FILE *)fd;
            return log_file;
        }
#endif
    }
    return NULL;
}

void mem_pool_log_close(void)
{
    if (log_file != NULL && log_file != INVALID_HANDLE_VALUE)
        _close((int)log_file);
}

int mem_pool_printf( char *fmt, ... )
{
    int n;
    char msg_buf[MAX_TEXT_LEN];
    char *new_fmt = NULL;
    va_list arglist;

    va_start(arglist, fmt);
#ifdef _MSC_VER
    new_fmt = str_replace(fmt, "\n", "\r\n");
    if (new_fmt != NULL) {
        n = _vsnprintf_s(msg_buf, _countof(msg_buf), MAX_TEXT_LEN - 1, new_fmt, arglist);
        free(new_fmt);
    }
    else
        n = _vsnprintf_s(msg_buf, _countof(msg_buf), MAX_TEXT_LEN - 1, fmt, arglist);
#else
    n = vsnprintf(msg_buf, MAX_TEXT_LEN - 1, fmt, arglist);
#endif
    if (n > 0)
        msg_buf[n] = '\0';

    if (log_file == NULL || log_file == INVALID_HANDLE_VALUE)
        mem_pool_log_init(DEFAULT_LOG_FILENAME);
    if (log_file != NULL && log_file != INVALID_HANDLE_VALUE)
        _write((int)log_file, msg_buf, strlen(msg_buf));

    new_fmt = str_replace(msg_buf, "%", "%%");
    if (new_fmt != NULL) {
        n = printf(new_fmt);
        free(new_fmt);
    }
    else
        n = printf(msg_buf);
    va_end(arglist);
    return n;
}

FORCE_INLINE
size_t get_rand()
{
#if defined(RAND_MAX) && (RAND_MAX == 0x7FFF)
    return (rand() | rand() << 15);
#else
    return rand();
#endif
}

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
    loop_count1     = MAX(1, loop_count1_ / 100);
    loop_count2     = MAX(0, loop_count2_ / 100);
    loop_count3     = MAX(0, loop_count3_ / 100);
#endif
}

mem_pool_test_param::~mem_pool_test_param()
{

}

void mem_pool_test_param::Setting( size_types size_type_, alloc_ways alloc_way_,
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
    loop_count1     = MAX(1, loop_count1_ / 100);
    loop_count2     = MAX(0, loop_count2_ / 100);
    loop_count3     = MAX(0, loop_count3_ / 100);
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
        size_t max_alloc = max_alloc_size - min_alloc_size + 1;
        for (i = 0; i < loop_count1; i++) {
            alloc_size = (i % max_alloc) + min_alloc_size;
            //alloc_size = MAX(1, alloc_size);
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
    else if (size_type == ST_RANDOM_SIZE && alloc_way == AW_REPEATED_ALLOC) {
        size_t rnd;
        size_t max_alloc = max_alloc_size - min_alloc_size + 1;
        for (i = 0; i < loop_count1; i++) {
            rnd = get_rand();
            alloc_size = (rnd % max_alloc) + min_alloc_size;
            //alloc_size = MAX(1, alloc_size);
            p = Malloc(alloc_size);
            if (p)
                Free(p);
        }
    }
}
