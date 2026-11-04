typedef struct Vec67260 {
    int x;
    int y;
    int z;
} Vec67260;

typedef struct Pick67260 {
    unsigned short c[3];
} Pick67260;

class Actor67260 {
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
    virtual void s12(void *a);
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
    virtual void s53(int a);
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
    virtual void s86(int a);
    char pad4[0x24];
    int x;
};

extern "C" {
extern const Pick67260 data_0209e140;
extern const Pick67260 data_0209e148;
extern void *func_02008444(void);
extern void *func_02010194(void *a);
extern void func_02058a80(Vec67260 *dst, void *src);
extern int func_0200f5c8(void *camera);
extern int func_020328f8(void *a);
extern void *func_0200847c(void);
extern void func_0202a0c0(void *a, int b);
void func_02067260(Actor67260 *p);
}

void func_02067260(Actor67260 *p)
{
    Vec67260 pos;
    Pick67260 right;
    Pick67260 left;
    unsigned short *list;
    void *spot;
    int limit;

    p->s53(0);
    p->s33(0x4a0);
    spot = func_02010194(func_02008444());
    if (spot != 0) {
        func_02058a80(&pos, spot);
        p->s12(&pos);
    }
    limit = func_0200f5c8(func_02008444()) + 0x80000;
    if (p->x > limit) {
        right = data_0209e140;
        list = right.c;
        p->s65(list[func_020328f8(func_02008444())]);
    } else {
        left = data_0209e148;
        list = left.c;
        p->s65(list[func_020328f8(func_02008444())]);
    }
    p->s86(1);
    func_0202a0c0(func_0200847c(), 0x12);
}
