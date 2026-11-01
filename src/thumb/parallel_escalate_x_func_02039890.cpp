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
    virtual void s8();
    virtual void s9();
    virtual void s10();
    unsigned char pad4[0xc];
    unsigned char mode;
    unsigned char pad11[0x17];
    unsigned char blend[0x99];
    unsigned char wait;
    unsigned char padc2[0x23];
    unsigned char step;
};

extern "C" {
extern void func_0203971c(Fader *p, int a);
extern void func_020379f8(Fader *p);
extern void func_02039558(void *a, int b, int c, int d);
extern void func_020394e4(void *a, int b);
void func_02039890(Fader *p);
}

void func_02039890(Fader *p)
{
    int n;

    if (p->step == 2) {
        func_0203971c(p, 1);
        func_020379f8(p);
        p->s10();
    }
    n = 10 - p->step;
    func_02039558(p->blend, 0x1000, (int)((1.0 - 0.1 * n) > 0 ? (1.0 - 0.1 * n) * 4096.0 + 0.5 : (1.0 - 0.1 * n) * 4096.0 - 0.5), 0);
    p->step++;
    p->wait = 1;
    if (p->step == 10) {
        func_02039558(p->blend, 0x1000, 0x1000, 0);
        func_020394e4(p->blend, 0);
        p->mode = 0x33;
        p->wait = 0x14;
        p->step = 0;
    }
}
