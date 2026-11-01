extern "C" {
int func_0203ad9c(void *p, unsigned int v);
}

struct Ctl {
    virtual void s0() = 0;
    virtual void s1() = 0;
    virtual void s2() = 0;
    virtual void s3() = 0;
    virtual void s4() = 0;
    virtual void s5() = 0;
    virtual void s6() = 0;
    virtual void s7() = 0;
    virtual void s8() = 0;
    virtual void s9() = 0;
    virtual void s10() = 0;
    virtual void s11() = 0;
    virtual void s12() = 0;
    virtual void s13() = 0;
    virtual void s14() = 0;
    virtual void s15() = 0;
    virtual void s16() = 0;
    virtual void s17() = 0;
    virtual void s18() = 0;
    virtual void s19() = 0;
    virtual void s20() = 0;
    virtual void s21() = 0;
    virtual void s22() = 0;
    virtual void s23() = 0;
    virtual void s24() = 0;
    virtual void s25() = 0;
    virtual void s26() = 0;
    virtual void s27() = 0;
    virtual void s28() = 0;
    virtual void s29() = 0;
    virtual void s30(int v) = 0;
    virtual unsigned short s31() = 0;
    int unk_04[3];
    unsigned char state;
    unsigned char unk_11[0xd8 - 0x11];
    unsigned short target;
    unsigned short prev;
    signed char mode;
    unsigned char arg;
    unsigned char unk_de[0xf0 - 0xde];
    unsigned char phase;
    unsigned char unk_f1[0xf8 - 0xf1];
    Ctl *child;
};

extern "C" void func_0203aaec(Ctl *s)
{
    int prev;
    int cur;
    int t;

    prev = s->prev;
    s->target = func_0203ad9c(s, s->arg);
    cur = s->target;
    if (cur == prev) {
        s->s30(0);
        s->state = 6;
        s->phase = 3;
        return;
    }
    if (s->mode == 1) {
        if (cur == 0) {
            t = prev + 0x500;
            if (t > 0x10000) {
                s->s30(cur);
            } else {
                s->s30((unsigned short)t);
            }
        } else {
            t = prev + 0x500;
            if (t < cur) {
                s->s30((unsigned short)t);
            } else {
                s->s30(cur);
            }
        }
    } else if (s->mode == -1) {
        if (cur == 0xeaaa) {
            if (prev == 0) {
                s->s30(0xfb00);
            } else if (prev > cur + 0x500) {
                s->s30((unsigned short)(prev - 0x500));
            } else {
                s->s30(cur);
            }
        } else {
            if (prev > cur + 0x500) {
                s->s30((unsigned short)(prev - 0x500));
            } else {
                s->s30(cur);
            }
        }
    }
    s->prev = s->s31();
    s->child->s30(s->prev);
    s->phase = 1;
}
