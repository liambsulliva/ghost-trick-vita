class Widget {
public:
    virtual void w0();
    virtual void w1();
    virtual void w2();
    virtual void w3();
    virtual void w4();
    virtual void w5();
    virtual void w6();
    virtual void w7();
    virtual void w8();
    virtual void w9();
    virtual void w10();
    virtual void w11();
    virtual void w12();
    virtual void w13();
    virtual void w14();
    virtual void w15();
    virtual void w16();
    virtual void w17();
    virtual void w18();
    virtual void w19();
    virtual void w20();
    virtual void w21();
    virtual void w22();
    virtual void w23();
    virtual void w24();
    virtual void w25();
    virtual void w26();
    virtual void w27();
    virtual void w28();
    virtual void w29();
    virtual void w30();
    virtual void w31();
    virtual void w32();
    virtual void w33();
    virtual void w34();
    virtual void w35();
    virtual void w36();
    virtual void w37();
    virtual void w38();
    virtual void w39();
    virtual void w40();
    virtual void w41();
    virtual void w42();
    virtual void w43();
    virtual void w44();
    virtual void w45();
    virtual void w46();
    virtual void w47();
    virtual void w48();
    virtual void w49();
    virtual void w50();
    virtual void w51();
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
    virtual void v8(int a);
    unsigned char pad4[0xc];
    unsigned char state;
    unsigned char timer;
    unsigned char pad12[0x32];
    int w44;
    unsigned char pad48[4];
    int f4c[2];
    void *w54;
    Widget *w58;
    int f5c;
    unsigned char b60;
};

extern "C" {
extern char data_020cfb44[];
extern char data_02084780[];
extern char data_020cf5dc[];
extern char data_0208478c[];
extern char data_02084794[];
extern char data_020847a8[];
extern char data_020847b4[];
extern char data_020847c8[];
extern char data_020cdc90[];
extern char data_020847e4[];
extern void func_0205429c(int obj);
extern void *func_0201003c(void);
extern void func_02015970(void *mgr);
extern void func_0202f64c(int *timer);
extern void *func_02008444(void);
extern void func_0200f084(void *game, int a, int b, int c, int d);
extern void func_02011d04(void *game, int a, int b);
extern void func_02010d18(void *game);
extern void *func_02009900(void);
extern int func_0202fa8c(void *list, char *name);
extern void *func_02008860(void);
extern void func_0200c82c(void *sys, int a);
extern int func_02027518(int id);
extern void func_020541d8(int obj, int id, int a, int b);
extern int func_02027d4c(char *name, int a);
extern void func_02015a24(void *mgr, int entry, int a);
extern void *func_02055b74(char *name);
extern void func_020153d8(void *obj, Scene *owner);
extern void func_0202f7e8(void *list, void *obj, int a, int b);
extern unsigned int func_02011c80(void *game);
extern int func_02012518(void *game, char *a, char *b);
extern void func_0201571c(void *obj, unsigned int n);
extern unsigned int func_02015728(void *obj);
extern void func_0201192c(void *game, int value);
extern void func_020521d4(char *name, int a, int b, int c);
extern unsigned int func_0201572c(void);
extern void func_02015748(void *mgr);
extern void func_02015288(void *game);
extern char *func_02011264(void);
extern char *func_0201123c(char *base, int index);
extern void func_02022f34(char *obj, char *name, int a);
extern void func_0200ec24(void *game, int a);
extern int func_02022fc0(char *obj, char *name);
extern void func_0200fe74(void *game);
void func_02015428(Scene *p);
}

void func_02015428(Scene *p)
{
    unsigned int n;
    unsigned int sel;
    int entry;

    switch (p->state) {
    case 0:
        func_0205429c(p->w44);
        func_02015970(func_0201003c());
        if (p->w58 != 0) {
            p->w58->w51();
        }
        func_0202f64c(p->f4c);
        p->state++;
        p->timer = 0x1e;
        break;
    case 1:
        if (p->timer == 4) {
            func_0200f084(func_02008444(), 1, 1, 1, 1);
            func_02011d04(func_02008444(), 4, 0);
            func_02010d18(func_02008444());
        }
        p->timer--;
        if (p->timer == 0) {
            func_0202f64c(&p->f5c);
            p->state++;
        }
        break;
    case 2:
        if (func_0202fa8c(func_02009900(), data_020cfb44) == 0) {
            func_0200c82c(func_02008860(), 1);
            if (func_02027518(0x100b) != 0) {
                func_020541d8(p->w44, 0x31, 0, 0);
            } else {
                func_020541d8(p->w44, 0x2b, 0, 0);
            }
            p->state++;
            p->timer = 0xf;
        }
        break;
    case 3:
        p->timer--;
        if (p->timer == 0) {
            entry = func_02027d4c(data_02084780, 2);
            if (entry != 0) {
                func_02015a24(func_0201003c(), entry, 0);
            }
            p->state++;
            p->timer = 0xf;
        }
        break;
    case 4:
        p->timer--;
        if (p->timer == 0) {
            p->w54 = func_02055b74(data_020cf5dc);
            if (p->w54 != 0) {
                func_020153d8(p->w54, p);
                func_0202f7e8(func_02009900(), p->w54, 7, 0);
                n = func_02011c80(func_02008444());
                if (n != 0 && func_02012518(func_02008444(), data_0208478c, data_02084794) != 0) {
                    p->b60 = 1;
                    n = 1;
                } else {
                    n++;
                }
                func_0201571c(p->w54, n);
            }
            p->state++;
        }
        break;
    case 5:
        sel = func_02015728(p->w54);
        switch (sel) {
        case 1:
            func_0201192c(func_02008444(), 0xf);
            func_020521d4(data_020847a8, 0, 0x1e, 0x100);
            if (p->b60 != 0) {
                p->state = 0x6f;
            } else {
                p->state = 0x63;
            }
            break;
        case 2:
        case 3:
        case 4:
            if (sel - 1 <= func_02011c80(func_02008444())) {
                func_020521d4(data_020847a8, 0, 0x1e, 0x100);
                p->state = 0x6f;
            }
            break;
        }
        break;
    case 0x63:
        if (func_0201572c() == 0) {
            func_02015748(func_0201003c());
            func_02015970(func_0201003c());
            func_02015288(func_02008444());
            func_0201192c(func_02008444(), 0x13);
            func_02022f34(func_0201123c(func_02011264(), 0), data_020847b4, 0);
            p->v8(0);
        }
        break;
    case 0x6f:
        if (func_0201572c() == 0) {
            func_0200f084(func_02008444(), 0, 0, 1, 1);
            func_02015748(func_0201003c());
            func_02015970(func_0201003c());
            func_02015288(func_02008444());
            func_0200ec24(func_02008444(), 0);
            func_02022f34(func_0201123c(func_02011264(), 0), data_020847c8, 0);
            p->state = 0x70;
        }
        break;
    case 0x70:
        if (func_02022fc0(func_0201123c(func_02011264(), 0), data_020cdc90) == 0) {
            func_0200fe74(func_02008444());
            func_02022f34(func_0201123c(func_02011264(), 0), data_020847e4, 0);
        }
        break;
    }
}
