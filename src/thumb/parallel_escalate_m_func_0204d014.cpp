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
    virtual void slot46(int value);
};

extern "C" {
extern char data_020aa6d8;
extern char data_020cfc30;
extern void func_02047120(void *self);
extern Object *func_02061cec(void *heap);
extern void func_0204c540(Object *object, int value);
extern void func_020153d8(Object *kid, void *owner);
extern void *func_02009900(void);
extern void func_0202f7e8(void *system, Object *object, int layer, int flags);
}

extern "C" char *func_0204d014(char *self)
{
    Object *child;
    func_02047120(self);
    *(void **)self = &data_020aa6d8;
    *(Object **)(self + 0xb8) = 0;
    *(unsigned short *)(self + 0xbc) = 0;
    *(unsigned short *)(self + 0xbe) = 0;
    *(int *)(self + 0xc0) = 0x19000;
    child = func_02061cec(&data_020cfc30);
    *(Object **)(self + 0xb8) = child;
    child->slot46(1);
    (*(Object **)(self + 0xb8))->slot33(0x610);
    func_0204c540(*(Object **)(self + 0xb8), 0x610);
    func_0202f7e8(func_02009900(), *(Object **)(self + 0xb8), 7, 0);
    func_020153d8(*(Object **)(self + 0xb8), self);
    return self;
}
