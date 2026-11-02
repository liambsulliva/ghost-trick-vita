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
    virtual void slot33();
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
};

struct Room {
    unsigned char pad0[0x4];
    unsigned char state;
    unsigned char mode;
    unsigned char pad6[0x2];
    int kind;
    unsigned char padc[0x14];
    void *handle0;
    void *handle1;
    int place;
    int start;
    unsigned char pad30[0x2];
    unsigned char ready;
    unsigned char pad33[0x3];
    unsigned char flag36;
    unsigned char flag37;
};

extern "C" {
extern char data_020cfb28[];
extern char data_020cfb90[];
extern char data_020cfaf0[];
extern char data_020cf87c[];
extern char data_020cf898[];
extern char data_020cfe48[];
extern char data_020cdd3c[];
extern char data_02099dfc[];
extern char data_02099dd4[];
extern char data_02099e84[];
extern char data_02099e00[];
extern void *func_02008444(void);
extern void *func_02009900(void);
extern int func_02011320(void *game);
extern int func_020328c4(void *game);
extern void func_0201192c(void *game, int value);
extern void func_020328d4(void *game, int value);
extern void func_02010974(void *game, int value);
extern void func_020141f4(void *game);
extern void func_0200e62c(void *game);
extern void func_0202fad4(void *system, char *name);
extern void *func_0205e8a8(char *name);
extern int func_020328e4(void *game);
extern void func_02061138(void *view, int value);
extern void func_0202f7e8(void *system, void *object, int a, int b);
extern int func_02010194(void *game);
extern void func_0200f958(void *game, int value);
extern void func_0200ee54(void *game, int value);
extern void *func_0205c01c(char *name);
extern int func_020328f8(void *game);
extern void func_0205e22c(void *object, unsigned char value);
extern void func_0202fbcc(unsigned int *it, int kind);
extern int func_0200eac4(unsigned int *it);
extern void *func_0200eab4(unsigned int *it);
extern Obj *func_0200eab8(unsigned int *it);
extern void func_0200eabc(unsigned int *it, int a);
extern int func_02008828(void *object, char *kind);
extern void func_02059020(Obj *object);
extern void func_02059128(Obj *object);
extern void func_0200ef98(void *game);
extern void func_02032900(void *object, int value);
extern void func_02032908(void *game);
extern void func_020135b8(void *game, unsigned char value);
extern void func_0200da04(char *a, char *b, char *c, char *d);
void func_020326a4(Room *p);
}

void func_020326a4(Room *p)
{
    unsigned int it[4];
    unsigned int list[4];
    void *object;
    int place;

    if (func_02011320(func_02008444()) != 0x15 || func_020328c4(func_02008444()) == 0) {
        func_0201192c(func_02008444(), 4);
    }
    func_020328d4(func_02008444(), 1);
    func_02010974(func_02008444(), 0);
    func_020141f4(func_02008444());
    func_0200e62c(func_02008444());
    func_0202fad4(func_02009900(), data_020cfb28);
    p->handle1 = func_0205e8a8(data_020cfb90);
    if (p->handle1 != 0) {
        if (func_020328e4(func_02008444()) != 0) {
            func_02061138(p->handle1, 0);
        }
        func_0202f7e8(func_02009900(), p->handle1, 1, 0);
    }
    place = func_02010194(func_02008444());
    p->place = place;
    p->start = place;
    if (p->kind == 1) {
        p->place = 0;
        p->ready = 1;
        func_0200f958(func_02008444(), 0x10000);
        func_0200ee54(func_02008444(), 1);
        if (p->handle1 != 0) {
            func_02061138(p->handle1, 0);
        }
    }
    object = func_0205c01c(data_020cfaf0);
    p->handle0 = object;
    if (object != 0) {
        func_0202f7e8(func_02009900(), object, 1, 0);
        func_0205e22c(p->handle0, func_020328f8(func_02008444()));
    }
    func_0202fbcc(it, 6);
    while (func_0200eac4(it) != 0) {
        if (func_02008828(func_0200eab4(it), data_020cf898) != 0) {
            func_02059020(func_0200eab8(it));
            if (func_02008828(func_0200eab4(it), data_020cf87c) != 0) {
                func_02059128(func_0200eab8(it));
            }
        }
        func_0200eabc(it, 0);
    }
    func_0202fbcc(list, 7);
    while (func_0200eac4(list) != 0) {
        if (func_02008828(func_0200eab4(list), data_020cfe48) != 0) {
            func_0200eab8(list)->slot46();
        }
        func_0200eabc(list, 0);
    }
    if (func_02008828(p, data_020cdd3c) == 0) {
        func_0200ef98(func_02008444());
    }
    p->mode = 0xf;
    p->flag36 = 0;
    p->state = 0;
    p->flag37 = 0;
    if (func_020328c4(func_02008444()) != 0) {
        func_02032900(p->handle0, 1);
    } else {
        func_02032908(func_02008444());
    }
    func_020135b8(func_02008444(), 0);
    if (func_02008828(p, data_020cdd3c) == 0) {
        func_0200da04(data_02099dfc, data_02099dd4, data_02099e84, data_02099e00);
    }
}
