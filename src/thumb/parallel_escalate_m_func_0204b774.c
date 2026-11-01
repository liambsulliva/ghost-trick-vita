struct Depth4b774 {
    int depth;
    int unused4;
    int unused8;
};

void *func_02008444(void);
int func_0202c394(void *a);
int func_0203435c(void *a);
int func_02010b54(unsigned char *a);
void *func_0202970c(void);
int func_02029708(void *a);
int func_0200f5e8(void *a);
int func_0200f5c8(void *a);
void func_02048734(void);
void func_02048740(int a, int b, int c);
void func_02048750(int a, int b, int c, int d, int e, int f, int g, int h);
void func_0204878c(int a, int b, int c, int d, int e, int f);
void func_020487b0(int a);
void func_020487bc(int a);
void func_020487c8(short x, short y, short z);
void func_020487e4(void);

void func_0204b774(unsigned char *p)
{
    int base;
    int hi;
    int lo;
    int top;
    int i;
    int idx;
    int fx;
    int d;
    short x;
    unsigned char cell;

    if (func_0202c394(func_02008444()) == 0 && func_0203435c(func_02008444()) == 0 &&
        p[0xc] == 3 && func_02010b54(p) != 0) {
        top = func_02029708(func_0202970c());
        base = -*(int *)(p + 0xc0) + func_0200f5e8(func_02008444()) + 0x60000;
        cell = (unsigned char)(func_0200f5c8(func_02008444()) >> 12);
        hi = cell >> 4;
        lo = cell & 15;
        func_02048734();
        func_02048740(0, 0, -(top - *(unsigned short *)(p + 0x42)));
        func_02048750(0, 0, 0, 0, 0, 0, 0, 0);
        func_0204878c(0, 0, 3, *(signed char *)(p + 0x45), *(short *)(p + 0xb6), 0);
        func_020487b0(2);
        for (i = 0; i < 18; i++) {
            idx = i + hi;
            if (idx >= 17) {
                idx -= 17;
            }
            fx = ((i << 4) - lo - 0x80) << 12;
            d = base + (*(struct Depth4b774 **)(p + 0xb8))[idx].depth;
            if (d > 0x84000) {
                d = 0x84000;
            }
            if (d < -0x84000) {
                d = -0x84000;
            }
            x = (short)(fx >> 7);
            func_020487bc(0x5ac6);
            func_020487c8(x, (short)((d / 3) >> 5), 0);
            func_020487bc(0x2102);
            func_020487c8(x, -0x1010, 0);
        }
        func_020487e4();
    }
}
