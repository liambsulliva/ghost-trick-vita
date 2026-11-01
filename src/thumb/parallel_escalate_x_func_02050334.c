void func_02050334(int format, int gen, int s, int t, int repeat, int flip, int color0, unsigned int address)
{
    *(volatile unsigned int *)0x040004a8 = (unsigned int)((address >> 3) | (format << 26) | (gen << 30) | (s << 20) | (t << 23) | (repeat << 16) | (flip << 18) | (color0 << 29));
}
