typedef struct Step458b0 {
    signed char next;
    char pad1;
    short limit;
    char pad4[8];
} Step458b0;

class Mover458b0 {
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
    virtual void s68(int a, Step458b0 *b, int c);
    char pad4[0x104];
    short pos;
    signed char index;
};

extern "C" {
extern unsigned char data_020cff60[];
int func_020458b0(Mover458b0 *p, int a, Step458b0 *steps);
}

int func_020458b0(Mover458b0 *p, int a, Step458b0 *steps)
{
    short limit;
    signed char next;

    p->pos += (short)(data_020cff60[1] + 1);
    limit = steps[p->index].limit;
    if (p->pos <= limit) {
        return 1;
    }
    p->pos = limit;
    if (limit < 0) {
        return 1;
    }
    next = steps[p->index].next;
    if (next == -2) {
        return 0;
    }
    if (next < 0) {
        p->index++;
    } else {
        p->index = next;
    }
    p->s68(a, steps, p->index);
    return 1;
}
