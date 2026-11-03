typedef struct {
    short x;
    short y;
} Pt;

class Trail {
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
    virtual void s33();
    virtual void s34();
    virtual void s35(int a);
    virtual void s36();
    virtual void s37(int a);
    unsigned char pad4[8];
    unsigned char state;
    unsigned char padd[0x37];
    signed char width;
    unsigned char pad45[0x43];
    int count;
    unsigned char pad8c[4];
    unsigned short color;
    unsigned char shown;
    unsigned char pad93;
    unsigned short base;
    unsigned char pad96[2];
    Pt *pts;
    unsigned char glow;
};

extern "C" {
extern void *func_02008444(void);
extern int func_0202c394(void *a);
extern int func_02010b54(Trail *p);
extern int func_0202c358(Trail *p);
extern void *func_0202970c(void);
extern int func_02029708(void *a);
extern void func_02063220(int a);
extern void func_02061fc4(void);
extern void func_02061fd0(int a, int b, int c);
extern void func_020571e4(Trail *p);
extern void func_02061fe0(int a);
extern void func_02061fec(int a);
extern void func_02061ff8(int a, int b);
extern void func_02062018(int a, int b, int c);
extern void func_02062034(void);
void func_0206386c(Trail *p);
}

void func_0206386c(Trail *p)
{
    int d;
    int v;
    int t;
    unsigned int i;
    unsigned int j;
    signed char w;

    if (func_0202c394(func_02008444()) != 0 || p->state != 3 || func_02010b54(p) == 0 || func_0202c358(p) == 0 || p->shown == 0 || p->count == 0) {
        return;
    }
    d = func_02029708(func_0202970c());
    func_02063220(2);
    func_02061fc4();
    func_02061fd0(0, 0, -(d - p->base));
    p->s37(0x3c);
    func_020571e4(p);
    func_02061fe0(2);
    func_02061fec(p->color);
    v = 0x1f;
    if (p->width > 0) {
        v = 0x20 / p->width;
    }
    t = 0x20 - v;
    j = i = 0;
    for (; i < 0x22; i++) {
        if (i & 1) {
            func_02061ff8(0, 0);
        } else {
            func_02061ff8(0, t << 12);
        }
        func_02062018(p->pts[i].x, p->pts[i].y, 0);
    }
    func_02062034();
    if (p->glow == 1) {
        w = p->width;
        p->s35(0x1f);
        func_02061fc4();
        func_02061fd0(0, 0, -(d - (p->base - 1)));
        p->s37(0x3b);
        func_020571e4(p);
        func_02061fe0(2);
        func_02061fec(0x7fff);
        for (j = 0; j < 0x22; j++) {
            if (j & 1) {
                func_02061ff8(0, 0x3f000);
            } else {
                func_02061ff8(0, 0);
            }
            func_02062018(p->pts[j].x, p->pts[j].y, 0);
        }
        func_02062034();
        p->width = w;
    }
}
