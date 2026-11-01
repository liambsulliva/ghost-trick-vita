struct Obj0204dc94 {
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8(int a);
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35(int a);
};

extern "C" {
void *func_02008444(void);
int func_0200f330(void *a);
int func_0207b694(int a, int b);
}

extern "C" void func_0204dc94(Obj0204dc94 *obj)
{
    unsigned char *p = (unsigned char *)obj;
    int n;

    if (func_0200f330(func_02008444()) == 0) {
        if (*(short *)(p + 0xb6) < 0) {
            *(short *)(p + 0xb6) = 0x1a;
        }
        obj->s35((signed char)*(short *)(p + 0xb6));
        if (p[0xbb] == 0) {
            n = *(short *)(p + p[0xba] * 2 + 0xc0);
            if (n > 0) {
                *(int *)(p + 0xbc) = *(int *)(p + 0xbc) + func_0207b694(*(p + p[0xba] + 0xc8) << 12, n);
            }
            *(short *)(p + 0xb8) = (short)(*(short *)(p + 0xb8) + 1);
            if (*(short *)(p + 0xb8) > *(short *)(p + p[0xba] * 2 + 0xc0) && p[0xba] < 3) {
                p[0xba]++;
                *(short *)(p + 0xb8) = 0;
                return;
            }
        } else {
            n = *(short *)(p + p[0xba] * 2 + 0xcc);
            if (n > 0) {
                *(int *)(p + 0xbc) = *(int *)(p + 0xbc) - func_0207b694(*(p + p[0xba] + 0xd4) << 12, n);
            }
            *(short *)(p + 0xb8) = (short)(*(short *)(p + 0xb8) + 1);
            if (*(short *)(p + 0xb8) > *(short *)(p + p[0xba] * 2 + 0xcc)) {
                if (p[0xba] < 3) {
                    p[0xba]++;
                    *(short *)(p + 0xb8) = 0;
                    return;
                }
                obj->s8(0);
            }
        }
    }
}
