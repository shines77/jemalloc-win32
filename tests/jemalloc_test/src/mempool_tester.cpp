
#include "mempool_tester.h"

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

#define MAX_TEXT_LEN            512
#define MAX_LOG_FILENAME_LEN    260

static FILE *log_file = NULL;
static char log_filename[MAX_LOG_FILENAME_LEN];

#ifndef PATH_MAX
#define PATH_MAX            1024
#endif

#ifndef MAX_APP_PATH
#ifdef _WIN32
  #define MAX_APP_PATH      MAX_PATH
#else
  #define MAX_APP_PATH      PATH_MAX
#endif
#endif  /* MAX_APP_PATH */

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
static
char *str_replace(const char *strbuf, const char *sstr, const char *dstr)
{
    char *new_str = NULL, *next;
    size_t len, str_len, src_len, dst_len;
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
static
char *str_replace(const char *str_buf, const char *src_str, const char *dst_str, bool repeat)
{
    char *new_str = NULL, *next;
    size_t len, str_len, src_len, dst_len;
    register unsigned int i = 0;
    char *dup_buf;

    if ((str_buf == NULL) || (src_str == NULL) || (dst_str == NULL))
        return NULL;

    dup_buf = (char *)str_buf;
    src_len = strlen(src_str);
    while (strstr(dup_buf, src_str) != NULL){
        dup_buf = strstr(dup_buf, src_str) + src_len;
        i++;
        if (!repeat)
            break;
    }

    if (i != 0) {
        dup_buf = (char *)str_buf;
        str_len = strlen(str_buf);
        dst_len = strlen(dst_str);
        len = (str_len - (src_len - dst_len) * i + 1) * sizeof(char);
        new_str = (char *)malloc(len);
        if (new_str != NULL) {
            //new_str[0] = '\0';
            memset(new_str, 0, len);
            while ((next = strstr(dup_buf, src_str)) != NULL) {
                new_str = strncat(new_str, dup_buf, (next - dup_buf));
                new_str = strcat(new_str, dst_str);
                dup_buf = next + src_len;
                if (!repeat)
                    break;
            }
            new_str = strcat(new_str, dup_buf);
            return new_str;
        }
    }
    else {
        str_len = strlen(str_buf);
        len = (str_len + 1) * sizeof(char);
        new_str = (char *)malloc(len);
        if (new_str != NULL) {
            new_str[0] = '\0';
            //memset(new_str, 0, len);
            new_str = strcpy(new_str, str_buf);
            return new_str;
        }
    }
    return new_str;
}

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

FILE *mempool_log_init(const char *filename)
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
        mempool_log_close();

        // _O_APPEND, _O_CREAT, _SH_DENYRW, _SH_DENYNO
#ifdef _MSC_VER
        err = _sopen_s((int *)&fd, filename, _O_CREAT | _O_TRUNC | _O_BINARY | _O_RDWR,
            _SH_DENYNO, _S_IREAD | _S_IWRITE);
        if (err == 0) {
            log_file = (FILE *)fd;
            return log_file;
        }
#else
        fd = open(filename, _O_CREAT | _O_TRUNC | _O_BINARY | _O_RDWR, _S_IREAD | _S_IWRITE);
        if (fd != -1) {
            log_file = (FILE *)fd;
            return log_file;
        }
#endif
    }
    return NULL;
}

void mempool_log_close(void)
{
    if (log_file != NULL && log_file != INVALID_HANDLE_VALUE)
        _close((int)log_file);
}

int mempool_printf( char *fmt, ... )
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
        mempool_log_init(DEFAULT_LOG_FILENAME);
    if (log_file != NULL && log_file != INVALID_HANDLE_VALUE)
        _write((int)log_file, msg_buf, (unsigned int)strlen(msg_buf));

    new_fmt = str_replace(msg_buf, "%", "%%");
    if (new_fmt != NULL) {
        n = printf(new_fmt);
        free(new_fmt);
    }
    else {
        n = printf(msg_buf);
    }
    va_end(arglist);
    return n;
}
