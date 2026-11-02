class Part {
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
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53(int a, int b, int c, int d, int e, int f, int g);
};

struct Gauge {
    char pad00[0x28];
    unsigned char b28;
    unsigned char b29;
    signed char b2a;
    signed char b2b;
    signed char b2c;
    signed char b2d;
    char pad2e[0xa];
    unsigned int flags;
    int f3c;
    int f40;
    int f44;
    Part *parts[12];
};

extern "C" {
extern char data_020ceccc;
extern Part *func_0204ed08(void *type);
extern void func_020153d8(Part *part, struct Gauge *gauge);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, Part *b, int c, int d);
void func_0204f648(struct Gauge *gauge);
}

void func_0204f648(struct Gauge *gauge)
{
    unsigned int i;

    for (i = 0; i < 12; i++) {
        gauge->parts[i] = func_0204ed08(&data_020ceccc);
        gauge->parts[i]->s33(0x100);
        func_020153d8(gauge->parts[i], gauge);
        func_0202f7e8(func_02009900(), gauge->parts[i], 1, 0);
    }
    gauge->parts[0]->s53(0, 0xad, 0xcd, 0x21e, 0x223, 0x250, 0x255);
    gauge->parts[1]->s53(0xe8, 0, -0x20, 0x5d, 0x5e, 0x67, 0x68);
    gauge->parts[2]->s53(0, 0xa8, 0xc8, 0x228, 0x22d, 0x25a, 0x25f);
    gauge->parts[3]->s53(0, 0, -0x20, 0x5b, 0x5c, 0x65, 0x66);
    gauge->parts[4]->s53(0xac, 0xad, 0xcd, 0x246, 0x24b, 0x246, 0x24b);
    gauge->parts[5]->s53(0xe8, 0, -0x20, 0x5f, 0x60, 0x5f, 0x60);
    gauge->parts[10]->s53(0xac, 0xad, 0xcd, 0x264, 0x269, 0x264, 0x269);
    gauge->parts[11]->s53(0, 0xad, 0xcd, 0x23c, 0x241, 0x23c, 0x241);
    gauge->parts[6]->s53(0x60, 0xa6, 0xc6, 0x61, 0x62, 0x61, 0x62);
    gauge->parts[7]->s53(0x60, 0xa6, 0xc6, 0x63, 0x64, 0x63, 0x64);
    gauge->parts[8]->s53(0xac, 0xad, 0xcd, 0x232, 0x237, 0x232, 0x237);
    gauge->parts[9]->s53(0, 0xad, 0xcd, 0x23c, 0x241, 0x23c, 0x241);
    gauge->b2b = gauge->b2a = gauge->b2c = 0;
    gauge->b2d = -1;
    gauge->b28 = 0;
    gauge->f3c = 0;
    gauge->f40 = 0;
    gauge->b29 = 0;
    gauge->f44 = 0;
    gauge->flags = -1;
    gauge->flags &= ~0xe0;
}
