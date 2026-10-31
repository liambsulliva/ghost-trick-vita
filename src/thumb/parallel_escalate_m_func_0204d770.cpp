class Child0204d770 {
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
    virtual void s33();
    virtual void s34();
    virtual void s35(int a);
};

struct Obj0204d770 {
    char pad0[0x28];
    char part[0x8e];
    short fb6;
    Child0204d770 *c0;
    Child0204d770 *c1;
    Child0204d770 *c2;
    Child0204d770 *c3;
    Child0204d770 *c4;
    Child0204d770 *c5;
    Child0204d770 *c6;
};

extern "C" {
extern int data_020ceaa4;
void *func_02008444(void);
int func_0200f330(void *a);

void func_0204d770(Obj0204d770 *p)
{
    if (func_0200f330(func_02008444()) != 0) {
        return;
    }
    if (p->fb6 < 0) {
        p->fb6 = 0x12;
    }
    data_020ceaa4 = p->fb6;
    p->c0->s35(0x1a);
    p->c0->s12(p->part);
    p->c1->s35(0x1a);
    p->c1->s12(p->part);
    p->c2->s35(0x1a);
    p->c2->s12(p->part);
    p->c3->s35(0x1a);
    p->c3->s12(p->part);
    p->c4->s35(0x10);
    p->c4->s12(p->part);
    p->c5->s35(0x1a);
    p->c5->s12(p->part);
    p->c6->s35((signed char)p->fb6);
}
}
