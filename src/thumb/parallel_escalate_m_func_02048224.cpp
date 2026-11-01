struct Child48224 {
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
    virtual void s91();
    virtual void s92();
    virtual void s93();
    virtual void s94();
    virtual void s95();
    virtual void s96();
};

struct Self48224 {
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8(int a);
    char pad04[0x84];
    unsigned short h88;
    char pad8a[0x2e];
    int fb8;
    Child48224 *child;
    int timer;
    int fc4;
    int handle;
    unsigned char done;
};

extern "C" {
extern char data_020cea6c[];
void *func_02008444(void);
void *func_02009900(void);
Child48224 *func_02046fa4(void *heap);
void func_02048344(Child48224 *a, int b);
int func_02031b44(void *a);
void func_0204834c(Child48224 *a, int b);
void func_020153d8(Child48224 *a, Self48224 *b);
void func_0202f7e8(void *a, Child48224 *b, int c, int d);
int func_02031af4(void *a, int b);
void func_02031b04(void *a, int *b);
}

extern "C" void func_02048224(Self48224 *p)
{
    Child48224 *child = p->child;
    int handle;

    if (child == 0) {
        if (++p->timer >= 24) {
            p->fb8 = 0;
            child = func_02046fa4(data_020cea6c);
            p->child = child;
            func_02048344(child, (unsigned short)(p->h88 - 0x8032));
            handle = func_02031b44(func_02008444());
            p->handle = handle;
            p->child->s60(handle);
            func_0204834c(p->child, p->fc4);
            p->child->s33(0x48c);
            func_020153d8(p->child, p);
            func_0202f7e8(func_02009900(), p->child, 7, 0);
            p->timer = 0;
            return;
        }
    } else if (child != 0 && p->done == 0) {
        if (++p->timer >= 150) {
            child->s96();
            p->done = 1;
            return;
        }
    } else if (p->done != 0) {
        if (func_02031af4(func_02008444(), p->handle) != 0) {
            p->child = 0;
            func_02031b04(func_02008444(), &p->handle);
            p->s8(0);
        }
    }
}
