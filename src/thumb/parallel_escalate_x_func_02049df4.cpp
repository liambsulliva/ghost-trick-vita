class Icon {
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
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60(int a);
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65(int a);
};

class Label {
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
    virtual void s51(int a);
};

struct Panel {
    unsigned char pad0[0x28];
    int x;
    int y;
    int z;
    unsigned char pad34[0x84];
    unsigned char mode;
    unsigned char padb9[3];
    int handle;
    Icon * icon;
    Label * label;
    unsigned char padc8[0x50];
    short top;
    short base;
    short dx;
    short dy;
};

extern "C" {
extern char data_020cfe2c[];
extern char data_020cf724[];
extern void *func_02008444(void);
extern int func_0200f5c8(void *a);
extern int func_0200f5e8(void *a);
extern Icon *func_02065ab8(char *a);
extern int func_02031b44(void *a);
extern void func_020153d8(void *a, struct Panel *b);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, void *b, int c, int d);
extern Label *func_0205693c(char *a);
extern void func_0200ee34(Label *a, int b, int c, int d);
void func_02049df4(struct Panel *p);
}

void func_02049df4(struct Panel *p)
{
    p->dx = (p->x - func_0200f5c8(func_02008444())) >> 12;
    p->dy = (p->y - func_0200f5e8(func_02008444())) >> 12;
    if (p->mode == 3) {
        p->top = p->dx + (p->z >> 12);
        p->base = p->dy + (p->z >> 12);
    } else {
        p->base = 0xbf - (p->z >> 12);
    }
    if (p->mode != 3) {
        p->icon = func_02065ab8(data_020cfe2c);
        p->handle = func_02031b44(func_02008444());
        p->icon->s65(0x20);
        p->icon->s33(0x200);
        p->icon->s60(p->handle);
        func_020153d8(p->icon, p);
        func_0202f7e8(func_02009900(), p->icon, 1, 0);
    }
    (p->label = func_0205693c(data_020cf724))->s51(3);
    p->label->s33(0xa90);
    func_0200ee34(p->label, 4, 0x1f, 0x1f);
    p->label->s13(0x98000, (0x60 - p->base) << 12, 0);
    func_020153d8(p->label, p);
    func_0202f7e8(func_02009900(), p->label, 1, 0);
}
