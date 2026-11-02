class Stepper {
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
    virtual void finish();
    unsigned char pad4[0xc];
    unsigned char state;
    unsigned char pad11[0x9f];
    int a_target;
    int b_target;
    unsigned char padb8[0x9];
    unsigned char wait;
    unsigned char padc2[0x2];
    int a_cur;
    int b_cur;
    unsigned char padcc[0x11];
    unsigned char id;
    signed char phase;
    signed char last;
    signed char delta;
    unsigned char flag;
};

extern "C" {
extern void func_02039294(Stepper *p, int v);
extern int func_02008444();
extern unsigned int func_02038a84(int object);
extern void func_020396ac(Stepper *p, int mode);
extern void func_02039774(Stepper *p);
extern void func_02039890(Stepper *p);
extern void func_020399c8(Stepper *p);
extern void func_02039414(Stepper *p);
extern void func_020397a0(Stepper *p);
void func_02039074(Stepper *p);
}

void func_02039074(Stepper *p)
{
    if (p->wait != 0) {
        p->wait--;
        return;
    }

    if (p->state == 0) {
        func_02039294(p, 0xfa000);
        if (p->id == func_02038a84(func_02008444())) {
            func_020396ac(p, 1);
        }
        p->state = 1;
        return;
    }

    if (p->state == 2) {
        if (p->a_target > p->a_cur) {
            func_02039294(p, -0x8000);
        } else {
            p->finish();
            p->state = 0x33;
        }
    }

    if (p->state == 3) {
        if (p->a_target > p->a_cur) {
            func_02039294(p, -0xc000);
        } else {
            p->finish();
            p->state = 0x33;
        }
    }

    if (p->state == 5) {
        if (p->a_target < p->a_cur + 0xfa000) {
            func_02039294(p, 0x12000);
        } else {
            func_02039774(p);
            p->state = 6;
        }
    }

    if (p->state == 4) {
        if (p->a_target < p->a_cur + 0xfa000) {
            func_02039294(p, 0xc000);
        } else {
            func_02039774(p);
            p->state = 6;
        }
    }

    if (p->state == 0x32) {
        func_02039890(p);
        return;
    }

    if (p->state == 0x38) {
        func_020399c8(p);
        return;
    }

    if (p->state == 0x35) {
        p->phase = p->phase + p->delta;
        p->last = p->delta;
        func_02039414(p);
        p->state = 0x36;
    }

    if (p->state == 0x34) {
        p->phase = p->phase + p->delta;
        p->last = p->delta;
        func_02039414(p);
        p->a_target = p->a_cur;
        p->b_target = p->b_cur;
        p->last = 0;
        p->flag = 0;
        p->state = 0x33;
    }

    if (p->state == 0x37) {
        p->state = 0x33;
    }

    if (p->state == 0x36) {
        func_020397a0(p);
    }
}
