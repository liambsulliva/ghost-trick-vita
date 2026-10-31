extern void func_02017110(unsigned char *p, int s);
extern int func_0201e04c(int a, int b, int c, int d);

int func_02019a64(unsigned char *p)
{
    unsigned short *s;
    unsigned int c;

    *(unsigned short *)(p + 0x5e) = 0xfffe;
    p[0x3a] = 0;
    p[0x3b] = 0;
    *(unsigned short *)(p + 0x3c) = 0;
    *(unsigned short *)(p + 0x34) = 0;
    *(short *)(p + 0x36) = *(short *)(p + 0x36) + p[0x72];
    if (p[0x4e] != 0) {
        p[0x4e] = 0;
        *(int *)(p + 0x54) = 0;
    }
    s = (unsigned short *)(*(int *)(p + 0x14) + 2);
    for (;;) {
        c = *s;
        if (c < 0xff00) {
            break;
        }
        if (c == 0xff02 || c == 0xfffe) {
            return 0;
        }
        s++;
    }
    if (*(int *)(p + 0x54) == 1) {
        p[0x4f]++;
    }
    if (func_0201e04c(*(int *)(p + 0x6c), p[0x70], *(unsigned short *)(p + 0x36), 0) == 0) {
        p[0x74] = 1;
    }
    *(short *)(p + 0x36) = *(short *)(p + 0x36) + p[0x51];
    func_02017110(p, *(int *)(p + 0x14) + 2);
    return 0;
}
