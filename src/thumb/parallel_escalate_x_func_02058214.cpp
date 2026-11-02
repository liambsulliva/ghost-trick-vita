class Flash {
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
    virtual void s67();
    virtual void s68();
    virtual void s69();
    virtual void s70();
    virtual void s71();
    virtual void s72();
    virtual void s73();
    virtual void s74();
    virtual void s75();
    virtual void s76();
    virtual void s77();
    virtual void s78();
    virtual void s79();
    virtual void s80();
    virtual void s81();
    virtual void s82();
    virtual void s83();
    virtual void s84();
    virtual void s85();
    virtual void s86();
    virtual void s87();
    virtual void s88();
    virtual void s89();
    virtual void s90();
    virtual void s91(int a, int b, int c, signed char d);
    unsigned char pad4[0xd5];
    unsigned char flags;
    unsigned char padda[0x2];
    short timer;
    unsigned char state;
    unsigned char paddf[0x2];
    unsigned char dead;
};

extern "C" {
extern void *func_02008444(void);
extern int func_0200f330(void *game);
extern int func_02011320(void *game);
extern void func_02057fa4(Flash *p);
extern int func_02058414(Flash *p);
void func_02058214(Flash *p);
}

void func_02058214(Flash *p)
{
    if (p->dead != 0) {
        p->s8(0);
        return;
    }
    if (func_0200f330(func_02008444()) != 0 || func_02011320(func_02008444()) != 3) {
        p->s11();
        return;
    }
    p->s10();
    func_02057fa4(p);
    switch (p->state) {
    case 0:
        p->s13(0x58000 * (0x20 - p->timer) / 32, 0xc0000 * (0x20 - p->timer) / 32, 0);
        p->s91(0x1f, 0x1f, 0x1f, 0x1f * p->timer / 32);
        p->timer++;
        if (p->timer > 0x20) {
            p->timer = 0;
            p->state = 1;
        }
        break;
    case 4:
        p->s13(0x58000 * p->timer / 32, 0xc0000 * p->timer / 32, 0);
        p->s91(0x1f, 0x1f, 0x1f, 0x1f * (0x20 - p->timer) / 32);
        p->timer++;
        if (p->timer > 0x20) {
            p->s8(0);
        }
        break;
    case 1:
        p->timer++;
        if (p->timer > 1) {
            p->timer = 0;
            p->state = 2;
            p->flags = p->flags | 2;
        }
        break;
    case 3:
        p->timer++;
        if (p->timer > 1) {
            p->timer = 0;
            p->state = 4;
        }
        break;
    default:
        p->timer++;
        if (func_02058414(p) != 0 && p->timer > 0x50) {
            p->s13(0, 0, 0);
            p->s91(0x1f, 0x1f, 0x1f, 0x1f);
            p->state = 3;
            p->timer = 0;
            p->flags = p->flags & ~2;
        }
        break;
    }
}
