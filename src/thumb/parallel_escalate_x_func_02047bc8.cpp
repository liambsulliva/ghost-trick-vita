class Item {
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
};

class Owner {
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
    virtual void s65(int a);
    virtual void s66();
    virtual void s67();
    virtual void s68();
    virtual void s69();
    virtual void s70();
    virtual void s71();
    virtual int s72(char *a);
    virtual int s73(char *a);
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
    virtual void s87(int a);
};

struct Names {
    char *name[6];
};

extern "C" {
extern char data_020cf724[];
extern char data_0209b830[];
extern char data_0209b838[];
extern char data_0209b840[];
extern const Names data_0209b748;
extern void func_020130c8(void);
extern void *func_02008444(void);
extern unsigned char func_020265fc(void *a);
extern Item *func_0205693c(char *a);
extern void func_020153d8(Item *a, Owner *b);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, Item *b, int c, int d);
void func_02047bc8(Owner *p);
}

void func_02047bc8(Owner *p)
{
    Names names;
    unsigned int i;
    int v;
    int div;

    func_020130c8();
    p->s65(0x119);
    p->s87(1);
    ((short *)p)[0x71] = p->s73(data_0209b830);
    ((short *)p)[0x72] = p->s73(data_0209b838);
    ((unsigned char *)p)[0xe0] = func_020265fc(func_02008444());
    ((unsigned char *)p)[0x118] = 0xb;
    ((unsigned char *)p)[0x11d] = 0xa;
    v = ((int *)p)[0x55];
    if (v >= 1200) {
        ((unsigned char *)p)[0x118] = 0xc;
        v -= 1200;
    }
    if (v < 100) {
        v += 1200;
    }
    div = 1000;
    for (i = 1; i < 5; i++) {
        ((unsigned char *)p)[0x118 + i] = v / div;
        v = v % div;
        div = div / 10;
    }
    ((short *)p)[0x6e] = p->s72(data_0209b840);
    ((Item **)p)[0x36] = func_0205693c(data_020cf724);
    func_020153d8(((Item **)p)[0x36], p);
    ((Item **)p)[0x36]->s35(0);
    func_0202f7e8(func_02009900(), ((Item **)p)[0x36], 7, 0);
    names = data_0209b748;
    for (i = 0; i < 6; i++) {
        ((Item **)p)[0x3a + i] = func_0205693c(data_020cf724);
        func_020153d8(((Item **)p)[0x3a + i], p);
        ((Item **)p)[0x3a + i]->s35(0);
        func_0202f7e8(func_02009900(), ((Item **)p)[0x3a + i], 7, 0);
        ((short *)p)[0x80 + i] = p->s72(names.name[i]);
    }
    ((int *)p)[0x4b] = 0;
    ((int *)p)[0x48] = 0;
    ((int *)p)[0x51] = 0x8000;
    ((int *)p)[0x4e] = 0x8000;
    ((int *)p)[0x52] = 0;
    ((int *)p)[0x49] = 0;
    ((int *)p)[0x4f] = 0x8000;
    ((int *)p)[0x4c] = 0x8000;
    ((int *)p)[0x53] = 0x1000;
    ((int *)p)[0x50] = 0x1000;
    ((int *)p)[0x4d] = 0x1000;
    ((int *)p)[0x4a] = 0x1000;
}
