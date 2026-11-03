unsigned char func_0200a668(unsigned char *p)
{
    unsigned int n;
    unsigned char v;

    n = p[0x100] + 1;
    if ((n & 0xff) != p[0x101]) {
        v = p[p[0x100]];
        p[0x100] = n;
        return v;
    }
    return 0;
}
