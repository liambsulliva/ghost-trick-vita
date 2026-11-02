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
    virtual void s35(signed char value);
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual int s53();
};

class Menu {
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
    unsigned char timer;
    unsigned char pad6[0x32];
    Item *items[2];
    void *effect;
    int target;
    signed char sel;
    signed char kind;
};

struct Pad {
    unsigned int cur;
    unsigned int trig;
    unsigned int rel;
    unsigned int held;
};

extern "C" {
extern struct Pad data_020c9d50;
extern char data_020cfb44[];
extern char data_0209a9c4[];
extern void *func_02009900(void);
extern void *func_02008444(void);
extern void *func_02008860(void);
extern void *func_0200847c(void);
extern void *func_0201003c(void);
extern int func_0202fa8c(void *system, char *name);
extern void *func_020330b4(void *game);
extern void *func_02027684(void *game);
extern void *func_0202768c(void *game);
extern int func_02027698(void *object);
extern int func_02058b4c(void *object);
extern void func_0200c82c(void *object, int value);
extern void func_0203bfc8(Menu *p);
extern void *func_0202fb8c(void *system, char *name, int index);
extern void func_02061138(void *view, int value);
extern void func_02022f6c(Menu *p);
extern void func_0202a0c0(void *sound, int id);
extern void func_0203bfb4(Item *item, int mode);
extern void func_020542b0(void *effect);
extern void func_02015970(void *a);
extern void func_0202f64c(Item **handle);
extern void func_0203bfbc(Menu *p);
void func_0203bd84(Menu *p);
}

void func_0203bd84(Menu *p)
{
    void *object;
    unsigned int i;

    switch (p->state) {
    case 0:
        if (func_0202fa8c(func_02009900(), data_020cfb44) != 0) {
            break;
        }
        p->target = 0;
        p->kind = -1;
        object = func_020330b4(func_02008444());
        if (object != 0) {
            if (object == func_02027684(func_02008444()) || object == func_0202768c(func_02008444())) {
                p->target = func_02027698(object);
                p->kind = 1;
            } else if (func_02058b4c(object) != 0) {
                p->target = func_02027698(object);
                p->kind = 0;
            }
        } else {
            p->kind = 0;
        }
        func_0200c82c(func_02008860(), 1);
        func_0203bfc8(p);
        p->state = 3;
        break;
    case 3:
        break;
    case 0x87:
        if (p->timer == 0) {
            if ((data_020c9d50.trig & 0x1000) != 0) {
                p->timer = 0x1f;
            } else if ((data_020c9d50.trig & 0x2000) != 0) {
                p->sel = 1;
                p->timer = 0x1f;
            } else if ((data_020c9d50.held & 0x300) != 0) {
                func_0202a0c0(func_0200847c(), 1);
                p->sel = 1 - p->sel;
                func_0203bfb4(p->items[p->sel], 1);
                func_0203bfb4(p->items[1 - p->sel], 0);
            } else if (p->items[0]->s53() != 0) {
                p->sel = 0;
                p->timer = 0x1f;
            } else if (p->items[1]->s53() != 0) {
                p->sel = 1;
                p->timer = 0x1f;
            }
            if (p->timer == 0x1f) {
                if (p->sel == 0) {
                    func_0202a0c0(func_0200847c(), 2);
                } else {
                    func_0202a0c0(func_0200847c(), 3);
                }
                func_0203bfb4(p->items[p->sel], 2);
                func_0203bfb4(p->items[1 - p->sel], 0);
                func_020542b0(p->effect);
                p->effect = 0;
                func_02015970(func_0201003c());
            }
        }
        if (p->timer != 0) {
            for (i = 0; i < 2; i++) {
                if (p->items[i] != 0) {
                    p->items[i]->s35(p->timer);
                }
            }
            p->timer--;
            if (p->timer == 0) {
                for (i = 0; i < 2; i++) {
                    func_0202f64c(&p->items[i]);
                }
                if (p->sel == 1) {
                    func_0203bfbc(p);
                    return;
                }
                func_0200c82c(func_02008860(), 1);
                func_0203bfc8(p);
                p->state = 3;
            }
        }
        break;
    case 2:
        p->timer--;
        if (p->timer == 0) {
            p->s11();
            object = func_0202fb8c(func_02009900(), data_0209a9c4, -1);
            if (object != 0) {
                func_02061138(object, 1);
            }
            func_02022f6c(p);
        }
        break;
    }
}
