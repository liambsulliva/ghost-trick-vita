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
};

struct Room {
    unsigned char pad0[0x20];
    int handle0;
    int handle1;
    int place;
    unsigned char pad2c[0x4];
    unsigned char opened;
    unsigned char closed;
    unsigned char pad32;
    unsigned char seen;
    unsigned char quiet;
};

extern "C" {
extern char data_02099dfc[];
extern char data_02099dd4[];
extern char data_02099f5c[];
extern char data_02099e00[];
extern char data_02099f68[];
extern char data_02099f70[];
extern char data_02099f78[];
extern char data_02099f80[];
extern char data_020cf898[];
extern char data_020cfb28[];
extern void func_0202f64c(int *handle);
extern void func_0200da04(char *a, char *b, char *c, char *d);
extern void *func_02008444(void);
extern void *func_02009900(void);
extern void func_020328d4(void *game, int value);
extern void func_0202fbcc(unsigned int *it, int kind);
extern int func_0200eac4(unsigned int *it);
extern void *func_0200eab4(unsigned int *it);
extern Obj *func_0200eab8(unsigned int *it);
extern void func_0200eabc(unsigned int *it, int a);
extern int func_02008828(void *object, char *kind);
extern void func_02059020(Obj *object);
extern void func_02033324(Obj *object, int value);
extern void func_0200eff0(void *game);
extern int func_02027518(int flag);
extern int func_02011320(void *game);
extern Obj *func_0205c030(char *name);
extern int func_02012518(void *game, char *a, char *b);
extern void func_0200f170(Obj *object, int value);
extern void func_020117a8(void *game, int value);
extern void func_0202f7e8(void *system, Obj *object, int a, int b);
extern void func_0200f9e8(void *game, int value);
extern int func_0200fa54(void *game);
extern int func_02010194(void *game);
extern int func_02033340(void *game);
extern int func_0203334c(void *game);
extern void func_0200f61c(void *game, int a, int b, int c);
extern void func_0200f634(void *game, int value);
extern void func_02010974(void *game, int value);
void func_02033140(Room *p);
}

void func_02033140(Room *p)
{
    unsigned int it[4];
    Obj *object;

    func_0202f64c(&p->handle0);
    func_0202f64c(&p->handle1);
    if (p->closed != 0) {
        return;
    }
    func_0200da04(data_02099dfc, data_02099dd4, data_02099f5c, data_02099e00);
    func_020328d4(func_02008444(), 0);
    func_0202fbcc(it, 6);
    while (func_0200eac4(it) != 0) {
        if (func_02008828(func_0200eab4(it), data_020cf898) != 0) {
            object = func_0200eab8(it);
            func_02059020(object);
            func_02033324(object, 0);
            object->slot15();
            object->slot22();
        }
        func_0200eabc(it, 0);
    }
    func_0200eff0(func_02008444());
    if (func_02027518(0x1003) != 0) {
        return;
    }
    switch (func_02011320(func_02008444())) {
    case 0:
    case 0x13:
    case 0x14:
        return;
    case 8:
        func_0200da04(data_02099dfc, data_02099f68, data_02099f70, data_02099e00);
        return;
    }
    if (p->opened != 0) {
        return;
    }
    object = func_0205c030(data_020cfb28);
    if (object != 0) {
        if (p->seen != 0 || func_02012518(func_02008444(), data_02099f78, data_02099f80) != 0 || func_02011320(func_02008444()) == 2) {
            func_0200f170(object, 0);
        } else {
            func_020117a8(func_02008444(), 1);
        }
        func_0202f7e8(func_02009900(), object, 1, 0);
    }
    if (p->quiet == 0) {
        if (func_02012518(func_02008444(), data_02099f78, data_02099f80) != 0) {
            if (p->seen == 0) {
                func_0200f9e8(func_02008444(), 0x10000);
            }
        } else if (func_0200fa54(func_02008444()) != 0) {
            func_0200f9e8(func_02008444(), 0x10000);
        } else if (p->place == func_02010194(func_02008444())) {
            func_0200f61c(func_02008444(), func_02033340(func_02008444()), func_0203334c(func_02008444()), 0x4000);
        } else {
            func_0200f634(func_02008444(), 0x4000);
        }
    }
    func_02010974(func_02008444(), 1);
}
