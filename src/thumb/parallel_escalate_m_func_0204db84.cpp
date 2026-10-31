union Half0204db84 {
    short v[2][4];
};

union Byte0204db84 {
    unsigned char v[2][4];
};

class Obj0204db84 {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
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
    virtual void s33(int a);
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(int a, int b, int c, int d, int e, int f, int g);
    char pad0[0x84];
    unsigned short f88;
    char pad1[0x2e];
    short fb8;
    unsigned char fba;
    unsigned char fbb;
    int fbc;
    short fc0[4];
    unsigned char fc8[4];
    short fcc[4];
    unsigned char fd4[4];
};

extern "C" {
extern const Half0204db84 data_0209bac0;
extern const Byte0204db84 data_0209baa0;
extern const Half0204db84 data_0209bab0;
extern const Byte0204db84 data_0209baa8;
void func_0204dc90(Obj0204db84 *p, int a);

void func_0204db84(Obj0204db84 *p)
{
    Half0204db84 a;
    Half0204db84 c;
    Byte0204db84 b;
    Byte0204db84 d;
    unsigned int index;
    unsigned int i;

    p->s42(10, 0, 1, 0, 1, -1, 0);
    func_0204dc90(p, 0x800);
    p->s33(0x620);
    p->fb8 = 0;
    p->fbb = 0;
    p->fbc = 0;
    p->fba = 0;
    index = (unsigned short)(p->f88 - 0x803a);
    if (index > 1) {
        index = 1;
    }
    a = data_0209bac0;
    b = data_0209baa0;
    c = data_0209bab0;
    d = data_0209baa8;
    for (i = 0; i < 4; i++) {
        p->fc0[i] = a.v[index][i];
        p->fc8[i] = b.v[index][i];
        p->fcc[i] = c.v[index][i];
        p->fd4[i] = d.v[index][i];
    }
}
}
