class Panel {
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
    virtual void s13(int a, int b, int c);
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22(int a, int b, int c);
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
    virtual void s46(int a, int b);
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65();
    virtual void s66();
    virtual void s67(int a, int b, int c, int d, int e);
    virtual void s68(Panel *a, char *b, int c);
    unsigned char pad4[0xc];
    unsigned char state;
    unsigned char pad11[0x10a];
    unsigned char index;
    unsigned char pad11c[0x4];
    int resource;
    unsigned char pad124[0xc];
    unsigned char ready : 1;
    unsigned char low130 : 2;
    unsigned char kind : 2;
    unsigned char mode : 3;
    unsigned char pad131[0x3];
    int scale_x;
    int scale_y;
    int frames;
    int timer;
    int delay;
    int x;
    int y;
    int vx;
    int vy;
    int pad158;
    void *target;
};

struct Vec {
    int x;
    int y;
    int z;
};

struct Ids {
    unsigned short id[2];
};

extern "C" {
extern const Ids data_0209b530;
extern char data_0209b594[];
extern int func_02027424(int value);
extern void *func_02008444(void);
extern void *func_02045cec(void *a, int b);
extern void func_02058a80(Vec *out, void *object);
extern int func_0200f5c8(void *a);
extern int func_0200f5e8(void *a);
extern void func_0205765c(Panel *p, int a, int b, int c, int d);
void func_02045b38(Panel *p);
}

void func_02045b38(Panel *p)
{
    Vec a;
    Vec b;
    Ids ids;

    p->scale_x = 0x1000;
    p->frames = 0x29;
    p->scale_y = 0x1000;
    p->timer = 0;
    p->state = 0;
    p->ready = 1;
    if (func_02027424(p->resource) != 0) {
        p->index = 1;
    }
    p->target = func_02045cec(func_02008444(), 0);
    if (p->target != 0) {
        func_02058a80(&a, p->target);
        p->x = a.x - func_0200f5c8(func_02008444()) - 0x80000;
        func_02058a80(&b, p->target);
        p->y = -b.y + func_0200f5e8(func_02008444()) + 0x60000;
    }
    p->vx = 0;
    p->vy = 0x4e000;
    p->delay = 0x52;
    if (p->mode == 1) {
        p->scale_x = 0x4cd;
        p->scale_y = 0x4cd;
        p->ready = 0;
        p->delay = 0x52;
        p->kind = 1;
    } else {
        p->scale_x = 0x4cd;
        p->scale_y = 0x4cd;
    }
    func_0205765c(p, 0, 0, 0x80000, 0x20000);
    p->s46(0x80000, 0x20000);
    p->s13(p->x, p->y, 0);
    p->s22(p->scale_x, p->scale_x, 0x1000);
    p->s11();
    p->s33(0x570);
    ids = data_0209b530;
    p->s67(ids.id[p->index], 10, 0x40, 0x28, 3);
    p->s68(p, data_0209b594, 9);
}
