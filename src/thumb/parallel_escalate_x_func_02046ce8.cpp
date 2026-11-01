class Child {
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
};

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
    virtual void s8(int a);
    unsigned char pad4[0xc];
    unsigned char state;
    unsigned char pad11[0x17];
    Child *child;
    int handle;
    unsigned short timer;
};

extern "C" {
extern char data_0209b6c0[];
extern char data_0209b6c4[];
extern char data_0209b6d0[];
extern char data_0209b6d8[];
extern char data_0209b6e0[];
extern void *func_02008444(void);
extern int func_02031af4(void *a, int b);
extern void func_02031b04(void *a, int *b);
extern int func_0200f330(void *a);
extern int func_02011320(void *a);
extern void func_02046e30(Fader *p, unsigned short a);
extern unsigned int func_02046e24(void *a);
extern void func_02026854(void *a, int b);
extern void func_0200da04(char *a, char *b, char *c, char *d);
void func_02046ce8(Fader *p);
}

void func_02046ce8(Fader *p)
{
    if (func_02031af4(func_02008444(), p->handle) != 0) {
        p->child = 0;
        func_02031b04(func_02008444(), &p->handle);
    }
    if (func_0200f330(func_02008444()) != 0 || func_02011320(func_02008444()) != 3) {
        if (func_02011320(func_02008444()) != 2) {
            if (p->child != 0) {
                p->child->s11();
            }
        }
        return;
    }
    if (p->child != 0) {
        p->child->s10();
    }
    switch (p->state) {
    case 0:
    case 1:
    case 2:
        p->timer--;
        if (p->timer == 0) {
            func_02046e30(p, 0xec - p->state * 5);
            p->timer = func_02046e24(func_02008444());
            p->state++;
        }
        func_02026854(func_02008444(), 1);
        break;
    case 3:
        p->timer--;
        if (p->timer == 0) {
            func_02046e30(p, 0xdd);
            func_0200da04(data_0209b6c0, data_0209b6c4, data_0209b6d0, data_0209b6d8);
            p->timer = func_02046e24(func_02008444()) * 3 / 2;
            p->state++;
        }
        func_02026854(func_02008444(), 1);
        break;
    case 4:
        func_02026854(func_02008444(), 0);
        p->timer--;
        if (p->timer == 0) {
            func_0200da04(data_0209b6c0, data_0209b6c4, data_0209b6e0, data_0209b6d8);
            p->s8(0);
        }
        break;
    }
}
