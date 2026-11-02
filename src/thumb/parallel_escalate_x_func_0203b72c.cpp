class Item {
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
    virtual void s12(int a);
};

class Scene {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    unsigned char state;
    unsigned char wait;
    unsigned char b6;
    unsigned char b7;
    unsigned char pad8[0x18];
    Item *w20;
    void *w24;
    void *w28;
    unsigned char b2c;
    unsigned char pad2d;
    unsigned char b2e;
    unsigned char pad2f;
    int w30;
};

extern "C" {
extern char data_020cfb44[];
extern char data_0209a8f8[];
extern char data_0209a900[];
extern char data_020cec54[];
extern char data_0209a914[];
extern char data_0209a920[];
extern char data_0209a92c[];
extern char data_0209a934[];
extern char data_0209a938[];
extern char data_0209a940[];
extern char data_0209a954[];
extern char data_0209a970[];
extern void *func_02009900(void);
extern int func_0202fa8c(void *list, char *name);
extern void *func_02008860(void);
extern void func_0200c82c(void *sys, int a);
extern void *func_0202f624(int size);
extern Item *func_020563cc(void *mem);
extern void *func_02008444(void);
extern unsigned int func_02011c80(void *game);
extern int func_02012518(void *game, char *a, char *b);
extern void func_0201571c(Item *item, unsigned int n);
extern void func_0202f7e8(void *list, void *obj, int a, int b);
extern void *func_0204e2a0(char *name);
extern void func_0203ba84(void *obj, int a, int b, int c);
extern int func_02027518(int id);
extern void func_020541d8(void *voice, int id, int a, int b);
extern int func_02027d4c(char *name, int a);
extern void *func_0201003c(void);
extern void func_02015a24(void *mgr, int entry, int a);
extern int func_02015728(Item *item);
extern void func_020542b0(void *voice);
extern void func_02015970(void *mgr);
extern void func_02022f6c(Scene *p);
extern void func_020521d4(char *name, int a, int b, int c);
extern unsigned int func_0201572c(void);
extern void func_02011cdc(void *game, int on);
extern void *func_0200847c(void);
extern void func_0202a068(void *sys, int a);
extern void func_0202a16c(void *sys, int a);
extern char *func_02011264(void);
extern char *func_0201123c(char *base, int index);
extern void func_02022f4c(char *obj);
extern void func_0202f64c(void *handle);
extern void func_0200ec24(void *game, int a);
extern void func_0200da04(char *a, char *b, char *c, char *d);
extern void func_0202682c(void *game);
extern void func_02022f34(char *obj, char *name, int a);
extern void func_0200fe74(void *game);
extern int func_02022dc8(Scene *p);
void func_0203b72c(Scene *p);
}

void func_0203b72c(Scene *p)
{
    Item *obj;
    int entry;

    switch (p->state) {
    case 0:
        if (func_0202fa8c(func_02009900(), data_020cfb44) != 0) {
            break;
        }
        func_0200c82c(func_02008860(), 1);
        obj = (Item *)func_0202f624(0x44);
        if (obj != 0) {
            obj = func_020563cc(obj);
        }
        p->w20 = obj;
        if (p->w20 != 0) {
            if (func_02011c80(func_02008444()) != 0 && func_02012518(func_02008444(), data_0209a8f8, data_0209a900) != 0) {
                p->b2c = 1;
                func_0201571c(p->w20, 1);
            } else {
                func_0201571c(p->w20, func_02011c80(func_02008444()) + 1);
            }
            func_0202f7e8(func_02009900(), p->w20, 1, 0);
        }
        p->w28 = func_0204e2a0(data_020cec54);
        if (p->w28 != 0) {
            func_0203ba84(p->w28, -0x80000, 0, 0x1e);
            func_0202f7e8(func_02009900(), p->w28, 1, 0);
        }
        p->state = 0x80;
        p->wait = 0x18;
        break;
    case 0x80:
        p->wait--;
        if (p->wait == 0) {
            if (p->w24 != 0) {
                if (func_02027518(0x100b) != 0) {
                    func_020541d8(p->w24, 0x31, 0, 0);
                } else {
                    func_020541d8(p->w24, 0x2b, 0, 0);
                }
            }
            p->state = 0x82;
            p->wait = 0xc;
        }
        break;
    case 0x82:
        p->wait--;
        if (p->wait == 0) {
            entry = func_02027d4c(data_0209a914, 2);
            if (entry != 0) {
                func_02015a24(func_0201003c(), entry, 0);
            }
            p->state = 1;
        }
        break;
    case 1:
        p->w30 = func_02015728(p->w20);
        if (p->b2e == 0 && p->w30 == 5) {
            p->v11();
            func_020542b0(p->w24);
            func_02015970(func_0201003c());
            func_02022f6c(p);
            break;
        }
        if (p->b2e != 0 && p->w30 == 5) {
            p->w30 = 0;
        }
        if (p->w30 > 0) {
            p->state = 0x83;
            p->wait = 0;
            p->b6 = 0;
            p->b7 = 0x64;
            func_02015970(func_0201003c());
            func_020542b0(p->w24);
        }
        break;
    case 3:
        break;
    case 0x83:
        p->w20->s12(p->b7 << 12);
        p->b7 -= 4;
        if (p->b7 > 0x64) {
            p->b7 = 0;
        }
        p->wait++;
        if (p->wait >= 0x20) {
            switch (p->w30) {
            case 1:
                func_020521d4(data_0209a920, 0, 0x1e, 0x100);
                if (p->b2c != 0) {
                    p->state = 0x85;
                } else {
                    p->state = 0x84;
                }
                break;
            case 2:
            case 3:
            case 4:
                func_020521d4(data_0209a920, 0, 0x1e, 0x100);
                p->state = 0x85;
                break;
            }
        }
        break;
    case 0x84:
        if (func_0201572c() == 0) {
            func_02011cdc(func_02008444(), 0);
            func_0202a068(func_0200847c(), 0x3c);
            func_0202a16c(func_0200847c(), 0x3c);
            func_02022f4c(func_0201123c(func_02011264(), 1));
            if (func_02012518(func_02008444(), data_0209a8f8, data_0209a92c) != 0) {
                func_0202f64c(&p->w20);
                func_0200ec24(func_02008444(), 0);
                func_0200da04(data_0209a934, data_0209a8f8, data_0209a92c, data_0209a938);
                p->state = 3;
            } else {
                func_0202682c(func_02008444());
                func_02022f34(func_0201123c(func_02011264(), 0), data_0209a940, 0);
            }
        }
        break;
    case 0x85:
        if (func_0201572c() == 0) {
            func_0202a068(func_0200847c(), 0x3c);
            func_0202a16c(func_0200847c(), 0x3c);
            func_0202f64c(&p->w20);
            func_0200ec24(func_02008444(), 0);
            func_0200fe74(func_02008444());
            func_02022f34(func_0201123c(func_02011264(), 0), data_0209a954, 0);
            p->state = 0x86;
        }
        break;
    case 0x86:
        if (func_02022dc8(p) == 0) {
            func_02022f34(func_0201123c(func_02011264(), 0), data_0209a970, 0);
        }
        break;
    }
}
