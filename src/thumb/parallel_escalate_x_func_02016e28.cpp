class Text16e28 {
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
    char pad4[0x10];
    unsigned short *text;
    void *source;
    char pad1c[0xc];
    unsigned short cur;
    char pad2a[2];
    unsigned char speed;
    unsigned char countdown;
    unsigned short delay;
    char pad30[0x28];
    unsigned char shown;
    char pad59[5];
    unsigned short prev;
    char pad60[6];
    unsigned char drawn;
};

extern "C" {
extern char data_020b9d58[];
extern int func_02015d48(char *a, Text16e28 *t);
void func_02016e28(Text16e28 *t);
}

void func_02016e28(Text16e28 *t)
{
    if (t->source == 0) {
        return;
    }
    if (t->delay != 0) {
        t->delay--;
        return;
    }
    t->s93();
    for (;;) {
        t->cur = *t->text;
        if (t->cur > 0xff00) {
            if (func_02015d48(data_020b9d58, t) == 1) {
                return;
            }
            continue;
        }
        if (t->countdown != 0) {
            t->countdown--;
            return;
        }
        t->countdown = t->speed;
        t->s88();
        t->drawn = 1;
        t->shown = 1;
        t->prev = t->cur;
        t->text++;
    }
}
