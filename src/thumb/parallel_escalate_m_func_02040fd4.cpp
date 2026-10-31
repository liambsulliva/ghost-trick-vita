extern "C" {
void func_0200b598(void *a, int b, int c);
int func_02008444(void);
unsigned char func_020322d4(int a);
void func_02040e78(void *a);
}

struct Fx {
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
    unsigned char unk_04[0x10 - 0x04];
    unsigned char state;
    unsigned char unk_11[0x28 - 0x11];
    unsigned char blk28[0xb0 - 0x28];
    int pos;
    unsigned char unk_b4[0xbc - 0xb4];
    int kind;
    unsigned char unk_c0[0xcc - 0xc0];
    int base;
    unsigned char unk_d0[0xd4 - 0xd0];
    int ofs;
    unsigned char unk_d8[0xe0 - 0xd8];
    unsigned char flag;
};

extern "C" void func_02040fd4(Fx *s)
{
    int t;

    switch (s->state) {
    case 0:
        break;
    case 1:
        t = s->base + s->ofs;
        if (t < s->pos) {
            s->pos = s->pos - 0x8000;
            return;
        }
        if (t >= s->pos) {
            s->pos = t;
            s->state = 0;
        }
        break;
    case 2:
        t = s->base - 0x80000;
        if (t < s->pos) {
            s->pos = s->pos - 0x8000;
            return;
        }
        if (t >= s->pos) {
            s->state = 0;
        }
        break;
    case 3:
        s->pos = s->base + 0x120000;
        func_0200b598(s->blk28, s->kind, 15);
        s->flag = func_020322d4(func_02008444());
        if (s->flag != 0) {
            func_02040e78(s);
        }
        if (s->kind == 0x12f1) {
            s->s11();
        } else {
            s->s10();
        }
        s->state = 1;
        break;
    }
}
