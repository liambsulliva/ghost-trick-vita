struct Flags {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
};

extern void func_0205728c(int a, int b, int c, int d, int e, int f, int g, unsigned int h);
extern void func_020572c8(unsigned int value, int mode);
extern void func_020572e4(int a, int b, int c, int d, int e, unsigned int f);

void func_020571e4(unsigned char *p)
{
    int level;

    if (*(int *)(p + 0x48) != 0) {
        if (((struct Flags *)(p + 0x78))->b0) {
            func_0205728c(p[0x62], 1, p[0x60], p[0x61], 3, 0, 0, *(unsigned int *)(p + 0x50));
        }
        else {
            func_0205728c(p[0x62], 1, p[0x60], p[0x61], 3, 0, 1, *(unsigned int *)(p + 0x50));
        }
        if (!((struct Flags *)(p + 0x78))->b2 || !((struct Flags *)(p + 0x78))->b1) {
            func_020572c8(*(unsigned int *)(p + 0x54), p[0x62]);
        }
        else {
            func_020572c8(*(unsigned int *)(p + 0x58), p[0x62]);
        }
    }
    else {
        func_0205728c(0, 0, 0, 0, 0, 0, 0, 0);
    }
    level = *(signed char *)(p + 0x44);
    if (level > 0x1f) {
        level = 0x1f;
    }
    func_020572e4(0, 0, 3, *(signed char *)(p + 0x45), level, *(unsigned int *)(p + 0x64));
}
