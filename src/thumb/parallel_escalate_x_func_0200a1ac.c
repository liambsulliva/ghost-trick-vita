void func_0200a1ac(int p, int x, int y, short c)
{
    unsigned short *s;
    int i4;
    unsigned short u3;
    int bb;
    char *q;
    bb = *(unsigned char *)(p + x + y * 0x40 + 0x6e02);
    q = (char *)p + 0x202;
    i4 = (x * 2 + y * 0x100) * 2;
    u3 = (unsigned short)((unsigned int)(unsigned short)(bb * 0x1000));
    s = (unsigned short *)(q + i4);
    s[0] = u3 | (unsigned short)(c * 4);
    s[1] = ((unsigned short)(c * 4) + 1) | u3;
    s[0x80] = u3 | ((unsigned short)(c * 4) + 2);
    s[0x81] = u3 | ((unsigned short)(c * 4) + 3);
    return;
}
