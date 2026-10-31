extern void func_02037d6c(void *s);
extern void func_020384e8(void *o);
extern void func_02037ee0(void *s);
extern void func_020386c4(void *o);
extern void func_02037db4(void *s, int a, int b);
extern void func_02038090(void *s, int a);
extern int func_02027d4c(const void *d, int a);
extern int func_02008444(void);
extern void func_02015a54(int p, int r, int a);
extern int func_02011244(int p);
extern void func_02038508(void *o);
extern void func_02037ea0(void *s);
extern void func_020386dc(void *o);
extern void func_02037e04(void *s, int a, int b);
extern void func_02037d3c(void *s);
extern void func_0203833c(void *o);
extern void func_02038684(void *o);
extern void func_020373f8(void *s, int a, int b, int c);
extern const unsigned char data_0209a8a8[];

struct PhaseIf {
    unsigned char unk_00[5];
    unsigned char phase;
    unsigned char unk_06[0x32];
    void *obj;
    unsigned char unk_3c[0x1f];
    unsigned char tail;
};

void func_02037f20(struct PhaseIf *s)
{
    int r;

    switch (s->phase) {
    case 0:
        func_02037d6c(s);
        func_020384e8(s->obj);
        func_02037ee0(s);
        func_020386c4(s->obj);
        func_02037db4(s, 0x10, 0x480);
        s->phase++;
        s->tail = 0x10;
        break;
    case 1:
        func_02038090(s, 0);
        r = func_02027d4c(data_0209a8a8, 2);
        if (r != 0) {
            func_02015a54(func_02008444(), r, 1);
        }
        s->phase++;
        break;
    case 2:
        if (func_02011244(func_02008444()) == 0) {
            s->phase++;
        }
        break;
    case 3:
        func_02038090(s, 1);
        func_02038508(s->obj);
        func_02037ea0(s);
        func_020386dc(s->obj);
        func_02037e04(s, 0x10, 0x480);
        s->phase++;
        s->tail = 0x18;
        break;
    case 4:
        func_02037d3c(s);
        func_0203833c(s->obj);
        func_02038684(s->obj);
        func_020373f8(s, 8, 0, 0);
        s->phase = 0;
        break;
    }
}
