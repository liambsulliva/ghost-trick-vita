class Fader {
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
    virtual void s13();
    virtual void s14();
    virtual void s15(int value);
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
    virtual void s42(unsigned short id, int a, int b, int c, int d, int e, int f);
    virtual void s43();
    unsigned char pad4[0xc2];
    signed char state;
    signed char next;
    signed char step;
    unsigned char padc9;
    unsigned short id_a;
    unsigned short id_c;
    unsigned short id_b;
    unsigned short id_d;
    unsigned char padd2[0x6];
    int to;
    int from;
    unsigned char done;
    unsigned char auto_next;
    unsigned char alt;
    unsigned char pade3;
    unsigned char timer[4];
};

extern "C" {
extern void func_0204f2e0(Fader *p);
extern void func_0200acbc(void *timer);
extern void func_0204f290(Fader *p, int next, int value);
extern void func_02056cb8(Fader *p);
void func_0204ef1c(Fader *p);
}

void func_0204ef1c(Fader *p)
{
    switch (p->state) {
    case 0:
        p->s43();
        p->s11();
        p->s15(p->from);
        if (p->auto_next != 0) {
            p->state = 5;
        }
        break;
    case 1:
        if (p->alt != 0) {
            p->s42(p->id_b, 1, 1, 1, 0, -1, 0);
        } else {
            p->s42(p->id_a, 1, 1, 1, 0, -1, 0);
        }
        p->s10();
        p->s15(p->to);
        func_0204f2e0(p);
        break;
    case 2:
        if (p->alt != 0) {
            p->s42(p->id_b, 1, 1, 1, 0, -1, 0);
        } else {
            p->s42(p->id_a, 1, 1, 1, 0, -1, 0);
        }
        p->s10();
        p->step++;
        if (p->step > 8) {
            p->step = 8;
            p->state = 1;
            func_0200acbc(p->timer);
            p->done = 1;
        } else {
            p->s15(p->from + ((p->step * (p->to - p->from)) >> 3));
            func_0204f2e0(p);
        }
        break;
    case 3:
        if (p->alt != 0) {
            p->s42(p->id_b, 1, 1, 1, 0, -1, 0);
        } else {
            p->s42(p->id_a, 1, 1, 1, 0, -1, 0);
        }
        p->s10();
        p->step--;
        if (p->step < 0) {
            p->step = 0;
            p->state = 0;
            p->done = 1;
        } else {
            p->s15(p->from + ((p->step * (p->to - p->from)) >> 3));
            func_0204f2e0(p);
        }
        break;
    case 4:
        if (p->alt != 0) {
            p->s42(p->id_d, 1, 1, 1, 0, -1, 0);
        } else {
            p->s42(p->id_c, 1, 1, 1, 0, -1, 0);
        }
        p->s10();
        p->s15(p->to);
        p->step++;
        if (p->step > 0xc) {
            p->step = 8;
            p->state = -1;
            p->done = 1;
        }
        break;
    case 5:
        p->s8(0);
        p->state = -1;
        p->next = -1;
        return;
    }
    if (p->next != -1 && p->state == -1) {
        func_0204f290(p, p->next, -1);
    }
    func_02056cb8(p);
}
