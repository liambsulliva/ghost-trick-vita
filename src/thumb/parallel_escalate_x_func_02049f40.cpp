class Sprite {
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
    virtual void s13(int x, int y, int z);
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
    virtual void s33(int value);
    virtual void s34();
    virtual void s35(signed char value);
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
    virtual void s46(int x, int y);
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51(int value);
};

class Back {
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
    virtual void s13(int x, int y, int z);
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
};

class Curtain {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8(int value);
    unsigned char pad4[0xb4];
    unsigned char state;
    unsigned char padb9[0x3];
    int handle;
    Back *back;
    Sprite *front;
    Sprite *label;
    unsigned char padcc[0x40];
    signed char alpha;
    unsigned char pad10d[0x3];
    int scroll;
    int drop;
    unsigned char pad118[0x2];
    short left;
    short top;
    short bottom;
};

extern "C" {
extern char data_020cf724[];
extern void *func_02008444(void);
extern void *func_02009900(void);
extern Sprite *func_0205693c(char *name);
extern void func_0200ee34(Sprite *sprite, unsigned char r, unsigned char g, unsigned char b);
extern void func_020153d8(Sprite *sprite, Curtain *owner);
extern void func_0202f7e8(void *system, Sprite *sprite, int a, int b);
extern int func_02031af4(void *game, int handle);
extern void func_02031b04(void *game, int *handle);
extern void func_0204a228(Curtain *p);
void func_02049f40(Curtain *p);
}

void func_02049f40(Curtain *p)
{
    int pos;

    switch (p->state) {
    case 0:
        p->scroll += 0x5000;
        pos = 0x118000 - p->scroll;
        if (pos < p->top << 12) {
            pos = p->top << 12;
            p->state = 1;
            p->scroll = (0x118 - p->top) << 12;
            p->label = func_0205693c(data_020cf724);
            p->label->s51(2);
            p->label->s33(0x210);
            func_0200ee34(p->label, 4, 0x1f, 0x1f);
            p->label->s13((p->top - 0x80) << 12, (0x60 - p->left) << 12, 0);
            func_020153d8(p->label, p);
            func_0202f7e8(func_02009900(), p->label, 1, 0);
        }
        p->back->s13(pos - 0x7e800, 0x60000 - (p->left << 12), 0);
        p->front->s46(p->scroll, 0x2000);
        break;
    case 1:
        p->drop += 0x5000;
        pos = (p->left << 12) - p->drop;
        if (pos < p->bottom << 12) {
            pos = p->bottom << 12;
            p->state = 2;
            p->drop = (p->left - p->bottom) << 12;
            p->back->s51();
        }
        p->back->s13((p->top << 12) - 0x7e800, 0x60000 - pos, 0);
        p->label->s46(0x2000, p->drop);
        break;
    case 2:
        p->alpha -= 3;
        if (p->alpha < 0) {
            p->alpha = 0;
        }
        p->front->s35(p->alpha);
        p->label->s35(p->alpha);
        if (func_02031af4(func_02008444(), p->handle) != 0) {
            func_02031b04(func_02008444(), &p->handle);
            p->back = 0;
        }
        if (p->alpha == 0 && p->back == 0) {
            p->s8(0);
            return;
        }
        break;
    case 3:
        func_0204a228(p);
        break;
    }
}
