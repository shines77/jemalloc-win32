//#ifndef _strings_h_
//#define _strings_h_

/******************************************************************************/
#ifdef JEMALLOC_H_TYPES

#ifdef _MSC_VER

/* MSVC doesn't define ffs/ffsl. This dummy strings.h header is provided
 * for both */
#include <intrin.h>
#pragma intrinsic(_BitScanForward)

/*
 * MSVC doesn't define ffs/ffsl. and ffs/ffsl function name
 *  have a conflict when you use in C++ (Microsoft VC++),
 *  so define this to rename ffs/ffsl.
 */
#define ffs(x)          je_ffs(x)
#define ffsl(x)         je_ffsl(x)

#endif  /* !_MSC_VER */

#endif /* JEMALLOC_H_TYPES */
/******************************************************************************/
#ifdef JEMALLOC_H_STRUCTS

#endif /* JEMALLOC_H_STRUCTS */
/******************************************************************************/
#ifdef JEMALLOC_H_EXTERNS

#ifdef _MSC_VER

static unsigned char __BitScanForward(unsigned long *Index, unsigned long x)
{
    /* Determining if r is a power of 2 */
    unsigned long i = 0;
#ifdef _MSC_VER
    while ((x & 1UL) == 0) {
#else
    while ((x & 1lu) == 0) {
#endif
        x >>= 1;
        i++;
    }
    if (Index)
        *Index = i;
    return (unsigned char)(x != 0);
}

static __forceinline int je_ffsl(long x)
{
	unsigned long i = 0;

	if (_BitScanForward(&i, (unsigned long)x))
		return (i + 1);
	return (0);
}

static __forceinline int je_ffs(int x)
{
	return (je_ffsl(x));
}

#endif  /* !_MSC_VER */

#endif /* JEMALLOC_H_EXTERNS */
/******************************************************************************/
#ifdef JEMALLOC_H_INLINES

#endif /* JEMALLOC_H_INLINES */
/******************************************************************************/

//#endif  /* !_strings_h_ */
