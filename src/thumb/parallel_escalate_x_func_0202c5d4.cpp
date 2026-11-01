class Model {
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
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual int s40();
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
    virtual void s91();
    virtual void s92();
    virtual int s93();
};

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
    virtual void s35(int a);
};

class Panel {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7(int a, int b, int c);
    virtual int s8(char *a, void *b, int c, int d);
};

struct Flags {
    unsigned char shown : 1;
    unsigned char lit : 1;
};

extern "C" {
extern char data_020ca2b8[];
extern Model *func_0202c384(void *a);
extern void func_0206e79c(void *a, void *b, void *c);
extern void func_0202c700(Part *a, int b, char *c);
extern void func_020421d4(int a, int b, int c, int d, int e);
extern void func_020421e8(int a);
extern void func_0202c724(Part *a, int b, int c, int d, int e);
void func_0202c5d4(Panel *p, int a, int b, int c, unsigned short *count);
}

void func_0202c5d4(Panel *p, int a, int b, int c, unsigned short *count)
{
    Model *m;
    int s;
    int t;
    int i;

    if (((struct Flags *)((char *)p + 4))->shown == 0) {
        return;
    }
    m = func_0202c384(*(void **)((char *)p + 0x80));
    s = m->s40();
    t = m->s93();
    p->s7(a, b, c);
    func_0206e79c((char *)p + 0xd0, (char *)p + 0x4c, (char *)p + 0x4c);
    if (p->s8(data_020ca2b8, (char *)p + 0xa0, b, c) == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        func_0202c700((Part *)((char *)p + 0xf4), i, data_020ca2b8 + i * 8);
    }
    func_020421d4(*(int *)((char *)p + 0x1c0), ((struct Flags *)((char *)p + 4))->lit ? 1 : 0, *(unsigned short *)(*(char **)((char *)p + 0x80) + 0x184), s, t);
    if (*(int *)(*(char **)((char *)p + 0x80) + 0x1f4) == 0) {
        func_020421e8(*(int *)((char *)p + 0x1c0));
    }
    ((Part *)((char *)p + 0xf4))->s33(*count);
    (*count)--;
    func_0202c724((Part *)((char *)p + 0xf4), 0, *((unsigned char *)p + 0x20), *((unsigned char *)p + 0x21), *((unsigned char *)p + 0x22));
    ((Part *)((char *)p + 0xf4))->s35((signed char)((*(int *)((char *)p + 0x24) * 0x1f / 100) >> 12));
    ((Part *)((char *)p + 0xf4))->s7();
}
