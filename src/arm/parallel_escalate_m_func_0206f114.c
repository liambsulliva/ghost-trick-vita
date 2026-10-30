typedef struct {
    int x;
    int y;
    int z;
} VecFx32;

int func_0206f114(const VecFx32 *pSrc)
{
    long long tmp;

    tmp = (long long)pSrc->x * pSrc->x;
    tmp += (long long)pSrc->y * pSrc->y;
    tmp += (long long)pSrc->z * pSrc->z;
    tmp <<= 2;
    *(volatile unsigned short *)0x040002b0 = 1;
    *(volatile unsigned long long *)0x040002b8 = (unsigned long long)tmp;
    while (*(volatile unsigned short *)0x040002b0 & 0x8000) {
    }
    return (*(volatile int *)0x040002b4 + 1) >> 1;
}
