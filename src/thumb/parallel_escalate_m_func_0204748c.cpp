class Obj0204748c {
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
    virtual void slot33(int value);
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

extern "C" {
extern char data_020ce7d4;
extern char data_020cfe2c;
extern void *func_02009900(void);
extern void func_0202fad4(void *system,void *name);
extern Obj0204748c *func_02065ab8(void *name);
extern void *func_02008444(void);
extern void *func_02031b44(void *system);
extern void func_020153d8(Obj0204748c *object,void *owner);
extern void func_0202f7e8(void *system,Obj0204748c *object,int index,int flag);

void func_0204748c(unsigned char *p)
{
    int value;

    func_0202fad4(func_02009900(),&data_020ce7d4);
    *(Obj0204748c **)(p + 0xbc) = func_02065ab8(&data_020cfe2c);
    *(void **)(p + 0xb8) = func_02031b44(func_02008444());
    (*(Obj0204748c **)(p + 0xbc))->slot65(0x8f);
    (*(Obj0204748c **)(p + 0xbc))->slot33(0x480);
    (*(Obj0204748c **)(p + 0xbc))->slot60(*(void **)(p + 0xb8));
    func_020153d8(*(Obj0204748c **)(p + 0xbc),p);
    func_0202f7e8(func_02009900(),*(Obj0204748c **)(p + 0xbc),1,0);
    *(int *)(p + 0xcc) = 0;
    value = *(int *)(p + 0x28);
    *(short *)(p + 0xc0) = 0;
    if (value <= 0) {
        *(int *)(p + 0xd0) = 0x800;
    } else {
        if (value < 0x1000) {
            *(int *)(p + 0xd0) = 0x1f000 / value;
        } else {
            *(int *)(p + 0xd0) = 0x1f000 / (value >> 12);
        }
    }
    value = *(int *)(p + 0x2c);
    if (value <= 0) {
        *(int *)(p + 0xc4) = 0x5a;
        return;
    }
    if (value < 0x1000) {
        *(int *)(p + 0xc4) = value;
        return;
    }
    *(int *)(p + 0xc4) = value >> 12;
}
}
