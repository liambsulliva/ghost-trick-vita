unsigned short func_0200d3a0(void *param_1, int param_2)
{
    unsigned short b8;
    unsigned short r8;
    unsigned short g8;
    unsigned int sum;
    int iy;
    int iu;
    unsigned short u;
    unsigned short t;
    int it;

    b8 = (unsigned short)(((param_2 & 0x7c00) >> 10) << 3);
    r8 = (unsigned short)((param_2 & 0x1f) << 3);
    g8 = (unsigned short)(((param_2 & 0x3e0) >> 5) << 3);
    sum = 117 * b8 + (306 * r8 + 601 * g8);
    iy = (int)((sum << 3) >> 16) + 2;
    if (iy > 31) {
        iy = 31;
    }
    iy = (unsigned short)iy;
    t = (unsigned short)(iy - (iy >> 3));
    iu = (int)((((unsigned int)iy << 15) >> 16) + 2);
    if (iu > 31) {
        iu = 31;
    }
    u = (unsigned short)iu;
    it = (int)t + 2;
    if (it > 31) {
        it = 31;
    }
    return (unsigned short)((((unsigned int)it & 0xffff) << 5) | u | (iy << 10));
}
