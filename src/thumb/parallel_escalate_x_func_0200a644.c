void func_0200a644(unsigned char *p, unsigned char v)
{
    unsigned int t;

    if (v != 0) {
        t = p[0x101];
        if (p[0x100] != ((t + 1) & 0xff)) {
            p[0x101] = t + 1;
            p[t] = v;
        }
    }
}
