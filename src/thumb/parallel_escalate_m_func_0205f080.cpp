struct Iface {
    virtual void s0() = 0;
    virtual void s1() = 0;
    virtual void s2() = 0;
    virtual void s3() = 0;
    virtual void s4() = 0;
    virtual void s5() = 0;
    virtual void s6() = 0;
    virtual void s7() = 0;
    virtual void s8() = 0;
    virtual void s9() = 0;
    virtual void s10() = 0;
    virtual void s11() = 0;
    virtual void s12() = 0;
    virtual void s13(int, int, int) = 0;
    virtual void s14() = 0;
    virtual void s15() = 0;
    virtual void s16() = 0;
    virtual void s17() = 0;
    virtual void s18() = 0;
    virtual void s19() = 0;
    virtual void s20() = 0;
    virtual void s21() = 0;
    virtual void s22(int, int, int) = 0;
    virtual void s23() = 0;
    virtual void s24() = 0;
    virtual void s25() = 0;
    virtual void s26() = 0;
    virtual void s27() = 0;
    virtual void s28() = 0;
    virtual void s29() = 0;
    virtual void s30() = 0;
    virtual void s31() = 0;
    virtual void s32() = 0;
    virtual void s33() = 0;
    virtual void s34() = 0;
    virtual void s35(int) = 0;
};

extern "C" {
extern unsigned int * func_02008444();
extern int func_0205d398(int);
extern int func_0200f5c8(int);
extern int func_0200f5e8(int);
}

extern "C" void func_0205f080(unsigned char *p)
{
    unsigned int i;
    unsigned char *e;
    int x;
    int y;
    int t;
    Iface *obj;

    if (func_0205d398((int)func_02008444()) == 0) {
        obj = (Iface *)(p + 4);
        obj->s35(*(signed char *)(p + 0x712));
        for (i = 0; i < *(unsigned int *)(p + 0x70c); i++) {
            e = p + i * 4;
            if (0 < *(int *)(e + 0x2cc)) {
                if (p[0x710] != 0) {
                    x = *(int *)(e + 0xcc);
                    y = -*(int *)(e + 0x1cc);
                }
                else {
                    t = func_0200f5c8((int)func_02008444());
                    x = (*(int *)(e + 0xcc) - t) - 0x80000;
                    t = func_0200f5e8((int)func_02008444());
                    y = -*(int *)(e + 0x1cc);
                    y = y + t;
                    y = y + 0x60000;
                }
                obj->s13(x, y, 0);
                obj->s22(*(int *)(e + 0x2cc), *(int *)(e + 0x2cc), 0x1000);
                obj->s32();
            }
        }
    }
}
