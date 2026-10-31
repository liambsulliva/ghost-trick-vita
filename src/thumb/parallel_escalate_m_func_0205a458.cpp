extern "C" {
extern void func_0205a55c(int *, int, int, int);
extern void func_0205a564(unsigned char *, unsigned char *, unsigned char *, int *, int *, int);

void func_0205a458(unsigned char *p, int *a, int b, unsigned int mode)
{
    int loc[3];
    int v;
    int w;

    switch (mode) {
    case 1:
        v = *(int *)(p + 0x88) - a[0];
        w = *(int *)(p + 0x8c) - a[1];
        func_0205a55c(loc, v, w, 0);
        func_0205a564(p, p + 0xac, p + 0xcc, loc, a, b);
        *(int *)(p + 0x10) |= 1;
        break;
    case 2:
        v = *(int *)(p + 0x28) - a[0];
        w = *(int *)(p + 0x2c) - a[1];
        func_0205a55c(loc, v, w, 0);
        func_0205a564(p, p + 0xb4, p + 0xd8, loc, a, b);
        *(int *)(p + 0x10) |= 2;
        break;
    case 4:
        v = *(int *)(p + 0x7c) - a[0];
        w = *(int *)(p + 0x80) - a[1];
        func_0205a55c(loc, v, w, 0);
        func_0205a564(p, p + 0xbc, p + 0xe4, loc, a, b);
        *(int *)(p + 0x10) |= 4;
        break;
    case 8:
        v = *(signed char *)(p + 0xa0);
        *(int *)(p + 0xa8) = v << 12;
        func_0205a55c(loc, (v << 12) - a[0], 0, 0);
        func_0205a564(p, p + 0xc4, p + 0xf0, loc, a, b);
        *(int *)(p + 0x10) |= 8;
        break;
    }
}
}
