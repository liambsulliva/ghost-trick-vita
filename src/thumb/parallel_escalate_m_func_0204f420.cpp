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
    virtual int slot56();
};

extern "C" {
extern char data_020c9d50;
extern signed char data_0209beb4[];
extern signed char data_0209beb8[];
extern signed char data_0209bec0[];
extern signed char data_0209bec8[];
extern void *func_0200847c(void);
extern void func_0202a0c0(void *system, int value);
extern void func_0200da04(signed char *name, signed char *value, signed char *text, signed char *extra);
extern void func_0204f380(void *self);
}

extern "C" void func_0204f420(Object *self)
{
    if ((self->slot56() != 0 || (*(unsigned int *)(&data_020c9d50 + 4) & 0x2000) != 0) &&
        *((signed char *)self + 0xc6) == 1) {
        func_0202a0c0(func_0200847c(), 3);
        self->slot55();
    }
    if (*((signed char *)self + 0xc6) == 4 && *((signed char *)self + 0xc8) == 12) {
        func_0200da04(data_0209beb4, data_0209beb8, data_0209bec0, data_0209bec8);
    }
    func_0204f380(self);
}
