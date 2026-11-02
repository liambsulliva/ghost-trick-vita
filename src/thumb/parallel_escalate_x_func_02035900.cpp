class Object {
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
    virtual void slot12(int);
    virtual void slot13(int, int, int);
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
    virtual void slot33(int);
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
    virtual void slot60();
    virtual void slot61();
    virtual void slot62();
    virtual void slot63();
    virtual void slot64();
    virtual void slot65(int);
};

class Source {
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
    virtual int slot17();
};

struct Owner {
    unsigned char pad0[0x24];
    Source *source;
};

struct Scene {
    unsigned char pad0[0x10];
    unsigned char state;
    unsigned char shown;
    unsigned char pad12[0x16];
    Owner *owner;
    Object *first;
    Object *second;
};

extern "C" {
extern void *func_02010b7c(void);
extern int func_02010b64(void *a);
extern int func_020357a4(Owner *owner);
extern void *func_02009900(void);
extern int func_0202fa8c(void *system, char *name);
extern Object *func_02034f94(char *name);
extern Object *func_02034fbc(char *name);
extern void func_020153d8(Object *object, Scene *scene);
extern void func_0202f7e8(void *system, Object *object, int a, int b);
extern unsigned int func_02015188(void *a);
extern void func_0200da04(char *a, char *b, char *c, char *d);
extern void func_0202fad4(void *system, char *name);
extern unsigned int data_020c9d50[];
extern char data_020cda10[];
extern char data_020cda2c[];
extern char data_0209a388[];
extern char data_0209a430[];
extern char data_0209a43c[];
extern char data_0209a3a4[];
extern char data_0209a444[];
extern char data_0209a44c[];
void func_02035900(Scene *p);
}

void func_02035900(Scene *p)
{
    Source *source;

    func_02010b64(func_02010b7c());
    if (func_020357a4(p->owner) != 0) {
        if (func_0202fa8c(func_02009900(), data_020cda10) == 0) {
            p->first = func_02034f94(data_020cda10);
            if (p->first != 0) {
                func_020153d8(p->first, p);
                p->first->slot65(0x114);
                p->first->slot33(0x120);
                func_0202f7e8(func_02009900(), p->first, 7, 0);
            }
        }
        if (p->first != 0) {
            source = p->owner->source;
            if (source != 0) {
                p->first->slot12(source->slot17());
            }
        }
    }

    switch (p->state) {
    case 0:
        if ((data_020c9d50[0] & 0xf00) != 0 || func_02015188(func_02010b7c()) != 0) {
            func_0200da04(data_0209a388, data_0209a430, data_0209a43c, data_0209a3a4);
            p->state = 1;
            if (p->shown == 0 && func_02015188(func_02010b7c()) != 0) {
                p->shown = 1;
                func_0202fad4(func_02009900(), data_020cda2c);
            }
        }
        break;
    case 1:
        if ((data_020c9d50[0] & 0xf00) != 0 || func_020357a4(p->owner) != 0) {
            func_0200da04(data_0209a388, data_0209a430, data_0209a444, data_0209a3a4);
        } else {
            func_0200da04(data_0209a388, data_0209a430, data_0209a44c, data_0209a3a4);
            p->state = 0;
            if (p->first != 0) {
                p->first->slot51();
                p->first = 0;
            }
        }
        break;
    }

    if (p->shown == 0 && func_020357a4(p->owner) == 0 && func_0202fa8c(func_02009900(), data_020cda2c) == 0) {
        p->second = func_02034fbc(data_020cda2c);
        if (p->second != 0) {
            p->second->slot65(0x12d);
            p->second->slot33(0x120);
            p->second->slot13(0, 0, 0);
            func_0202f7e8(func_02009900(), p->second, 7, 0);
        }
    }
}
