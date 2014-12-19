
#ifndef _JEMALLOC_TEST_H_
#define _JEMALLOC_TEST_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "targetver.h"

#define _LANG_ID                1

#define LOCAL_LOG_FILENAME      "C:\\mempool_log.txt"
#define DEFAULT_LOG_FILENAME    "mempool_log.txt"

#ifndef MAX
#define MAX(a, b)           (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b)           (((a) < (b)) ? (a) : (b))
#endif

#ifdef _MSC_VER
#define FORCE_INLINE        static __forceinline
#define __INLINE            static __inline
#else
#define FORCE_INLINE        static inline
#define __INLINE            static inline
#endif

#ifdef __cplusplus
extern "C" {
#endif

//

#ifdef __cplusplus
}
#endif

#endif  /* _JEMALLOC_TEST_H_ */
