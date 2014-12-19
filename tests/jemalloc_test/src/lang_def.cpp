
#if defined(__linux__)
#include <locale.h>
#include <locale>
#include <strings.h>
#elif defined(_MSC_VER)
#include <windows.h>
#include <string.h>
#define strncasecmp     strnicmp
#endif

#include "lang_def.h"

int g_lang_id = LANG_NONE;

#if defined(__linux__)

/// <comment>
///
/// 参考: Linux下不区分大小写的strstr函数
///
/// http://blog.csdn.net/yasi_xi/article/details/7329987
///
/// </comment>

char *strstri_(const char *str, const char *str_sub)
{
    size_t len, len_sub;
    if (str == NULL || str_sub == NULL)
        return NULL;
    len = strlen(str);
    len_sub = strlen(str_sub);
    if (len_sub == 0 || len == 0 || len < len_sub)
        return NULL;

    while (*str) {
        if (strncasecmp(str, str_sub, len_sub) == 0)
            return (char *)str;
        ++str;
    }
    return NULL;
}

/// <comment>
///
/// 参考: How to get current locale of my environment?
///
/// http://stackoverflow.com/questions/12170488/how-to-get-current-locale-of-my-environment
/// http://blog.csdn.net/code_robot/article/details/5686229
/// http://msdn.microsoft.com/zh-cn/library/x99tb11d.aspx
///
/// </comment>

int get_sys_locale_id(void)
{
    char *locale;
    int lang_id = LANG_NONE;
    
    // 当 locale 为 "" 时, 根据环境的设置来设定 locale
    setlocale(LC_ALL, "");
    // 当 locale 为 NULL 时, 函数只做取回当前 locale 操作, 通过返回值传出, 并不改变当前 locale
    locale = setlocale(LC_ALL, NULL);
    // "zh_CN.xxxxx"
    if (strstri_(locale, "zh_cn") != NULL) {
        lang_id = LANG_ZH_CN;
    }
    // "zh_TW.xxxxx"
    else if (strstri_(locale, "zh_tw") != NULL) {
        lang_id = LANG_ZH_TW;
    }
    // "en_US.xxxxx"
    else {
        lang_id = LANG_EN_US;
    }
    return lang_id;
}

int get_user_locale_id(void)
{
    return get_sys_locale_id();
}

#elif defined(_MSC_VER)

/// <comment>
///
/// 参考: Language Identifier Constants and Strings
///
///       http://msdn.microsoft.com/en-us/library/dd318693%28v=vs.85%29.aspx
///
///   或: Windows Locale Codes Sorted by Locale ID (LCID), Hexadecimal
///
///       http://blog.csdn.net/cuixiping/article/details/5592291 (如果可以访问, 推荐看这个)
///
/// </comment>

int get_sys_locale_id(void)
{
    int lang_id = LANG_NONE;
    LCID locale_id;
    locale_id = GetSystemDefaultLCID();
    if (locale_id == 0x0804) {      /* Chinese (PRC) */
        lang_id = LANG_ZH_CN;
    }
    else if (locale_id == 0x0404    /* Chinese (Taiwan) */
        || locale_id == 0x0C04      /* Chinese (Hong Kong S.A.R.) */
        || locale_id == 0x1004      /* Chinese (Singapore) */
        || locale_id == 0x1404      /* Chinese (Macau S.A.R.) */
        ) {
        lang_id = LANG_ZH_TW;
    }
    else {
        lang_id = LANG_EN_US;       /* English (United States) is 0x0409 */
    }
    return lang_id;
}

int get_user_locale_id(void)
{
    int lang_id = LANG_NONE;
    LCID locale_id;
    locale_id = GetUserDefaultLCID();
    if (locale_id == 0x0804) {      /* Chinese (PRC) */
        lang_id = LANG_ZH_CN;
    }
    else if (locale_id == 0x0404    /* Chinese (Taiwan) */
        || locale_id == 0x0C04      /* Chinese (Hong Kong S.A.R.) */
        || locale_id == 0x1004      /* Chinese (Singapore) */
        || locale_id == 0x1404      /* Chinese (Macau S.A.R.) */
        ) {
        lang_id = LANG_ZH_TW;
    }
    else {
        lang_id = LANG_EN_US;       /* English (United States) is 0x0409 */
    }
    return lang_id;
}

#else  /* other unknown os */

int get_sys_locale_id(void)
{
    return LANG_EN_US;
}

int get_user_locale_id(void)
{
    return LANG_EN_US;
}

#endif  /* __linux__ */

int get_current_langid(void)
{
    if (g_lang_id == LANG_NONE)
        g_lang_id = get_user_locale_id();
    return g_lang_id;
}

int set_current_langid(int lang_id)
{
    int old_lang_id = g_lang_id;
    if (lang_id == LANG_USER || lang_id < 0) {
        g_lang_id = get_user_locale_id();
        return g_lang_id;
    }
    else if (lang_id == LANG_ZH_CN) {
        g_lang_id = LANG_ZH_CN;
    }
    else {
        g_lang_id = LANG_EN_US;
    }
    return old_lang_id;
}
