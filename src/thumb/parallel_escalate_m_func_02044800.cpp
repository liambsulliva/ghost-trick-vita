struct Obj44800 {
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
    virtual void s22(int a, int b, int c);
    char pad04[0x24];
    int x;
    int y;
    char pad30[0x97];
    unsigned char busy;
    char padc8[0xc];
    int fd4;
    int handle;
    char paddc[4];
    unsigned char flags;
    char pade1[0xd];
    signed char mode;
    signed char step;
    unsigned short sound;
};

extern "C" {
extern unsigned char data_020cff60[];
void *func_02008444(void);
void *func_0200847c(void);
int func_0200f5c8(void *a);
int func_0200f5e8(void *a);
void func_0202a114(void *a, int b, int c, int d);
int func_02031af4(void *a, int b);
void func_02031b04(void *a, int *b);
void func_0204476c(Obj44800 *a);
void func_02010b74(Obj44800 *a);
void func_02044d7c(Obj44800 *a, int b, int c);
}

extern "C" void func_02044800(Obj44800 *p)
{
    int scale;
    void *snd;
    int dx;

    if (p->busy == 0) {
        func_0204476c(p);
        if (func_02031af4(func_02008444(), p->handle) != 0) {
            func_02031b04(func_02008444(), &p->handle);
            p->fd4 = 0;
        }
        if ((((unsigned int)p->flags << 30) >> 31) != 0 && p->fd4 == 0) {
            func_02010b74(p);
        }
        if ((((unsigned int)p->flags << 30) >> 31) == 0) {
            scale = 0x1000;
            if (p->mode != 1) {
                p->step += (signed char)(data_020cff60[1] + 1);
                if (p->step > 8) {
                    if (p->mode == 2) {
                        p->s22(0, 0, 0x1000);
                        func_02010b74(p);
                        return;
                    }
                    func_02044d7c(p, 1, 0);
                }
                scale = p->step << 9;
                if (p->mode == 2) {
                    scale = 0x1000 - scale;
                }
                if (p->mode == 1) {
                    scale = 0x1000;
                }
            }
            if (p->sound != 0 && scale > 0x800) {
                snd = func_0200847c();
                dx = func_0200f5c8(func_02008444());
                func_0202a114(snd, p->sound, p->x + dx + 0x80000, -p->y + func_0200f5e8(func_02008444()) + 0x60000);
                p->sound = 0;
            }
            p->s22(scale, scale, 0x1000);
        }
    }
}
