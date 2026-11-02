class Obj {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual void slot23();
    virtual void slot24();
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void slot33(int a);
    virtual void slot34();
    virtual void slot35();
    virtual void slot36();
    virtual void slot37();
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42();
    virtual void slot43();
    virtual void slot44();
    virtual void slot45();
    virtual void slot46();
    virtual void slot47();
    virtual void slot48();
    virtual void slot49();
    virtual void slot50();
    virtual void slot51();
    virtual void slot52();
    virtual void slot53();
    virtual void slot54();
    virtual void slot55();
    virtual void slot56();
    virtual void slot57();
    virtual void slot58();
    virtual void slot59();
    virtual void slot60(int a);
    virtual void slot61();
    virtual void slot62();
    virtual void slot63();
    virtual void slot64();
    virtual void slot65(int a);
};

struct Scene {
    unsigned char pad0[4];
    union {
        unsigned char step;
        int all;
    } state;
    int mode;
    unsigned char padc[0x18];
    Obj *badge;
    Obj *meter;
    int handle0;
    int handle1;
    int handle2;
    int frame;
};

extern "C" {
extern char data_020cdd3c[];
extern char data_020cd7b4[];
extern char data_020cf800[];
extern char data_020cfe2c[];
extern void *func_02008444(void);
extern void *func_02009900(void);
extern int func_02031af4(void *manager, int handle);
extern void func_02031b04(void *manager, int *handle);
extern int func_02011320(void *game);
extern int func_02027518(int flag);
extern void *func_02031b20(Scene *p);
extern void func_02022f2c(void *object, char *table, int index);
extern void func_02011598(void *game);
extern int func_02031b24(void);
extern void func_02031b30(void *game, int value);
extern void func_02011690(void *game, int value);
extern void func_020117a8(void *game, int value);
extern void func_02031b88(Scene *p);
extern Obj *func_02057b40(char *name);
extern Obj *func_02065ab8(char *name);
extern int func_020283b0(void *game);
extern int func_02031b3c(void *game);
extern int func_02031b44(void *game);
extern void func_020587d0(Obj *object, int low, int high);
extern void func_0202f7e8(void *system, Obj *object, int a, int b);
void func_02031924(Scene *p);
}

void func_02031924(Scene *p)
{
    signed char high;
    signed char low;

    switch (p->state.step) {
    case 0:
        if (func_02031af4(func_02008444(), p->handle0) != 0) {
            p->state.step++;
            func_02031b04(func_02008444(), &p->handle0);
        }
        break;
    case 1:
        if (func_02011320(func_02008444()) == 12 && func_02027518(0x1022) == 0) {
            func_02022f2c(func_02031b20(p), data_020cdd3c, 2);
            func_02011598(func_02008444());
            return;
        }
        if (func_02011320(func_02008444()) == 13 && func_02031b24() != 0) {
            func_02022f2c(func_02031b20(p), data_020cdd3c, 3);
            func_02011598(func_02008444());
            return;
        }
        func_02022f2c(func_02031b20(p), data_020cd7b4, 0);
        if (p->mode == 0) {
            func_02031b30(func_02008444(), 1);
            func_02011690(func_02008444(), 0);
        }
        func_020117a8(func_02008444(), 1);
        break;
    case 2:
        if (p->frame == 0x1b) {
            func_02031b88(p);
        }
        if (p->frame == 0) {
            p->meter = func_02057b40(data_020cf800);
            if (p->meter != 0) {
                high = func_020283b0(func_02008444());
                low = func_02031b3c(func_02008444());
                if (low < 0) {
                    low = 0;
                }
                if (high < low) {
                    low = high;
                }
                p->handle2 = func_02031b44(func_02008444());
                p->meter->slot60(p->handle2);
                func_020587d0(p->meter, low, high);
                func_0202f7e8(func_02009900(), p->meter, 7, 0);
            }
        }
        if (p->frame == 0x3c) {
            p->badge = func_02065ab8(data_020cfe2c);
            if (p->badge != 0) {
                p->handle1 = func_02031b44(func_02008444());
                p->badge->slot60(p->handle1);
                p->badge->slot65(0x2b);
                p->badge->slot33(0x460);
                func_0202f7e8(func_02009900(), p->badge, 7, 0);
            }
        }
        p->frame++;
        if (func_02031af4(func_02008444(), p->handle0) != 0 && func_02031af4(func_02008444(), p->handle1) != 0 && func_02031af4(func_02008444(), p->handle2) != 0) {
            p->state.all = 1;
        }
        break;
    }
}
