class Slide44270 {
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
    virtual void s42(int id, int a, int b, int c, int d, int e, int f);
    char pad4[0xc];
    unsigned char state;
    char pad11[2];
    unsigned char delay;
    char pad14[0x14];
    int pos;
    int pos2;
    char pad30[0x98];
    int kind;
    char padcc[4];
    int edge;
    char padd4[4];
    int home;
    int home2;
};

extern "C" {
void func_02044270(Slide44270 *p);
}

void func_02044270(Slide44270 *p)
{
    if (p->delay != 0) {
        p->delay--;
        return;
    }
    if (p->state == 3) {
        return;
    }
    if (p->state == 0) {
        p->pos = p->home;
        p->pos2 = p->home2;
        p->state = 1;
        p->delay = 5;
        return;
    }
    if (p->state == 1) {
        if (p->edge > p->home) {
            if (p->pos < p->edge) {
                p->pos += 0x3000;
            } else {
                p->pos = p->edge;
                p->state = 4;
            }
        } else if (p->edge < p->home) {
            if (p->pos > p->edge) {
                p->pos -= 0x3000;
            } else {
                p->pos = p->edge;
                p->state = 4;
            }
        }
        return;
    }
    if (p->state == 2) {
        if (p->edge > p->home) {
            if (p->pos > p->home) {
                p->pos -= 0x3000;
            } else {
                p->pos = p->home;
                p->state = 6;
            }
        } else if (p->edge < p->home) {
            if (p->pos < p->home) {
                p->pos += 0x3000;
            } else {
                p->pos = p->home;
                p->state = 6;
            }
        }
        return;
    }
    if (p->state == 5) {
        if (p->kind == 1) {
            p->s42(0x26e, 1, 1, 0, 0, -1, 0);
        } else if (p->kind == 2) {
            p->s42(0x278, 1, 1, 0, 0, -1, 0);
        }
        p->state = 4;
    }
}
