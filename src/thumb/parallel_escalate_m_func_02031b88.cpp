class Obj02031b88 {
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
    virtual void slot60(void *part);
    virtual void slot61();
    virtual void slot62();
    virtual void slot63();
    virtual void slot64();
    virtual void slot65(int value);
};

struct Self02031b88 {
    char pad[8];
    int index;
    char pad0c[0x14];
    Obj02031b88 *object;
    char pad24[8];
    void *part;
};

extern "C" {
extern char data_020cfe2c;
extern unsigned short data_02099d08[];
extern Obj02031b88 *func_02065ab8(void *name);
extern void *func_02008444(void);
extern void *func_02031b44(void *object);
extern void *func_02009900(void);
extern void func_0202f7e8(void *system, Obj02031b88 *object, int index, int flag);

void func_02031b88(Self02031b88 *self)
{
    self->object = func_02065ab8(&data_020cfe2c);
    if (self->object != 0) {
        self->part = func_02031b44(func_02008444());
        self->object->slot60(self->part);
        self->object->slot65(data_02099d08[self->index]);
        func_0202f7e8(func_02009900(), self->object, 7, 0);
    }
}
}
