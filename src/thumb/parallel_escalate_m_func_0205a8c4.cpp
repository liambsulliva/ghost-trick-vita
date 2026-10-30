class Object {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int);
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
};

extern "C" {
extern int data_020aeafc;
extern void func_0202f64c(...);
extern int func_02007bd8(...);
extern Object *func_02007b70(...);
extern void func_02042b20(...);
extern void func_02042774(...);
extern void func_0202f5e4(...);
}

extern "C" char *func_0205a8c4(char *param_1)
{
    unsigned int i;
    int *slots;
    Object *child;

    *(int *)param_1 = (int)&data_020aeafc;
    func_0202f64c(param_1 + 0x38);
    func_0202f64c(param_1 + 0x3c);
    func_0202f64c(param_1 + 0x40);
    i = 0;
    slots = (int *)(param_1 + 0x44);
    do {
        func_0202f64c(&slots[i]);
        i++;
    } while (i < 0x20);
    child = *(Object **)(param_1 + 0x34);
    if (child != 0) {
        child->slot54();
        *(Object **)(param_1 + 0x34) = 0;
    }
    if (*(int *)(param_1 + 0x11c) != 0 && func_02007b70(func_02007bd8()) != 0) {
        func_02007b70(func_02007bd8())->slot4(*(int *)(param_1 + 0x11c));
        *(int *)(param_1 + 0x11c) = 0;
    }
    func_02042b20(param_1 + 0xc8);
    func_02042774(param_1 + 0xc8);
    func_0202f5e4(param_1);
    return param_1;
}
