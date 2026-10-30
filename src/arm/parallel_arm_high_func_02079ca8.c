void func_02079ca8(int *s)
{
    unsigned short *p;
    p = (unsigned short *)0x04000204;
    s[0] = (*p & 12) >> 2;
    s[1] = (*p & 16) >> 4;
    *p = (*p & ~12) | 12;
    *p = *p & ~16;
}
