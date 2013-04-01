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

static unsigned char __BitScanForward2(unsigned long *Index, unsigned long x)
{
    unsigned long y, u;
    unsigned long i0, i1, i2, i3, i4;
    unsigned long i;

    if ( x == 0 )
        return 0;

    // all bits left of lsb become 1, lsb & right become 0
    y = x ^ (-(long)x);

    // XOR a shifted copy recovers a single 1 in the lsb's location
    u = y ^ (y >> 1);

    // .. and isolate the bit in log2 of number of bits
    i0 = (u & 0xAAAAAAAAULL) ?  1 : 0;
    i1 = (u & 0xCCCCCCCCULL) ?  2 : 0;
    i2 = (u & 0xF0F0F0F0ULL) ?  4 : 0;
    i3 = (u & 0xFF00FF00ULL) ?  8 : 0;
    i4 = (u & 0xFFFF0000ULL) ? 16 : 0;
    i = i4 | i3 | i2 | i1 | i0;

    if (Index)
        *Index = i;
    return (unsigned char)(x != 0);
}

static unsigned char __BitScanForward64(unsigned long *Index, uint64_t n)
{
    unsigned long i;
    if ( n == 0 )
        return 0;

    i = 0;
    n ^= n - 1;
    if ( n >= 1ull<<32 ) { n >>= 32; i+=32; }
    if ( n >= 1ull<<16 ) { n >>= 16; i+=16; }
    if ( n >= 1ull<< 8 ) { n >>= 8;  i+= 8; }
    if ( n >= 1ull<< 4 ) { n >>= 4;  i+= 4; }
    if ( n >= 1ull<< 2 ) { n >>= 2;  i+= 2; }
    if ( n >= 1ull<< 1 ) {           i+= 1; }
    if (Index)
        *Index = i;
    return (unsigned char)(n != 0);
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
