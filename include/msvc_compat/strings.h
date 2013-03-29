#ifndef strings_h
#define strings_h

/* MSVC doesn't define ffs/ffsl. This dummy strings.h header is provided
 * for both */
#include <intrin.h>
#ifndef _MSC_VER
#pragma intrinsic(_BitScanForward)
#endif

#ifdef _MSC_VER
#if 0
/*
 * Get the least significant 1 bit (LS1B), bit scan forward
 */
static int _BitScanForward2(unsigned long v, long x)
{
    /* Determining if r is a power of 2 */
    unsigned long r = v & -(long)v;
    return r;
}
#endif
#endif

static __forceinline int ffsl(long x)
{
	unsigned long i;

	if (_BitScanForward(&i, x))
		return (i + 1);
	return (0);
}

static __forceinline int ffs(int x)
{

	return (ffsl(x));
}

#endif
