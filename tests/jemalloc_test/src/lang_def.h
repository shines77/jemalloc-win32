
#ifndef _JEMALLOC_LANG_DEF_H_
#define _JEMALLOC_LANG_DEF_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#define LANG_USER       (-2)
#define LANG_NONE       (-1)
#define LANG_EN_US      0
#define LANG_ZH_CN      1
#define LANG_ZH_TW      2

#define LANG_ID         LANG_ZH_CN

#define MAX_LANG_ID     2

#ifdef __cplusplus
extern "C" {
#endif

extern int g_lang_id;

#if defined(__linux__)

char *strstri_(const char *str, const char *sub_str);

#endif  /* __linux__ */

// 获得当前系统的语言locale id
int get_sys_locale_id(void);

// 获得当前用户的语言locale
int get_user_locale_id(void);

// 获取当前用户的lang_id
int get_current_langid(void);

// 设置当前用户的lang_id
int set_current_langid(int lang_id);

#ifdef __cplusplus
}
#endif

#endif  /* _JEMALLOC_LANG_DEF_H_ */
