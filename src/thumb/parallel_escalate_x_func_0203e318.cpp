class Scene {
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
    unsigned char state;
    unsigned char wait;
    unsigned char pad6[0x1a];
    unsigned char quiet;
    unsigned char pad21[3];
    int handle;
};

extern "C" {
extern char data_020cfe64[];
extern char data_020cfb44[];
extern char data_0209acc4[];
extern char data_0209acc8[];
extern char data_0209acd4[];
extern char data_0209acdc[];
extern char data_0209ace4[];
extern void *func_02009900(void);
extern int func_0202fa8c(void *a, char *b);
extern void *func_02008444(void);
extern void func_0200f084(void *a, int b, int c, int d, int e);
extern void func_0200da04(char *a, char *b, char *c, char *d);
extern int func_0203e458(int a);
extern void func_020521d4(char *a, int b, int c, int d);
extern void *func_0200847c(void);
extern void func_0202a068(void *a, int b);
extern int func_0201572c(void);
extern void func_0202f64c(int *a);
extern void *func_0201003c(void);
extern void func_02015990(void *a, int b);
extern void func_02015970(void *a);
extern void func_0201192c(void *a, int b);
extern int func_02008860(void);
extern void func_0200c82c(int a, int b);
void func_0203e318(Scene *p);
}

void func_0203e318(Scene *p)
{
    switch (p->state) {
    case 0:
        p->wait--;
        if (p->wait == 0) {
            p->state = 1;
        }
        break;
    case 1:
        if (func_0202fa8c(func_02009900(), data_020cfe64) == 0) {
            func_0200f084(func_02008444(), 1, p->quiet == 0, 1, 1);
            p->state = 2;
        }
        break;
    case 2:
        if (func_0202fa8c(func_02009900(), data_020cfb44) == 0) {
            func_0200da04(data_0209acc4, data_0209acc8, data_0209acd4, data_0209acdc);
            p->state = 3;
        }
        break;
    case 5:
        if (p->handle != 0 && func_0203e458(p->handle) > 0) {
            func_020521d4(data_0209ace4, 0, 0x1e, 0x100);
            func_020521d4(data_0209ace4, 1, 0x1e, 0x100);
            if (p->quiet == 0) {
                func_0202a068(func_0200847c(), 0x1e);
            }
            p->state = 7;
        }
        break;
    case 7:
        if (func_0201572c() == 0) {
            func_0202f64c(&p->handle);
            func_0200f084(func_02008444(), 0, 0, 1, 1);
            func_02015990(func_0201003c(), 0);
            func_02015970(func_0201003c());
            func_0201192c(func_02008444(), 0);
            func_0200c82c(func_02008860(), 1);
            p->s11();
        }
        break;
    }
}
