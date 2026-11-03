typedef struct {
    int x;
    int y;
    int z;
} VecFx32;

static inline void set_div64(long long numer, long long denom)
{
    *(volatile unsigned short *)0x04000280 = 2;
    *(long long *)0x04000290 = numer;
    *(long long *)0x04000298 = denom;
}

static inline void set_sqrt64(unsigned long long param)
{
    *(volatile unsigned short *)0x040002b0 = 1;
    *(unsigned long long *)0x040002b8 = param;
}

void func_0206f174(const VecFx32 *pSrc, VecFx32 *pDst)
{
    long long t;
    int root;

    t = (long long)pSrc->x * pSrc->x;
    t += (long long)pSrc->y * pSrc->y;
    t += (long long)pSrc->z * pSrc->z;
    set_div64(0x0100000000000000LL, t);
    set_sqrt64((unsigned long long)(t << 2));
    while (*(volatile unsigned short *)0x040002b0 & 0x8000) {
    }
    root = *(volatile int *)0x040002b4;
    while (*(volatile unsigned short *)0x04000280 & 0x8000) {
    }
    t = *(volatile long long *)0x040002a0;
    t = t * root;
    pDst->x = (int)((t * pSrc->x + (1LL << 44)) >> 45);
    pDst->y = (int)((t * pSrc->y + (1LL << 44)) >> 45);
    pDst->z = (int)((t * pSrc->z + (1LL << 44)) >> 45);
}
