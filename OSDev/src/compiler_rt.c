#include "compiler_rt.h"
#include <stdint.h>

typedef unsigned long long u64;
typedef long long s64;

static u64 udiv64(u64 n, u64 d)
{
    u64 q = 0;
    u64 r = 0;

    for (int i = 63; i >= 0; i--)
    {
        r <<= 1;
        r |= (n >> i) & 1ULL;

        if (r >= d)
        {
            r -= d;
            q |= 1ULL << i;
        }
    }

    return q;
}

static u64 abs64(s64 x)
{
    return x < 0 ? ((u64)(-(x + 1)) + 1) : (u64)x;
}

s64 __divdi3(s64 a, s64 b)
{
    int neg = ((a < 0) != (b < 0));

    u64 ua = abs64(a);
    u64 ub = abs64(b);

    u64 q = udiv64(ua, ub);

    return neg ? -(s64)q : (s64)q;
}
