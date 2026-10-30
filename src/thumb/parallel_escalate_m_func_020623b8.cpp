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
    virtual void slot35(int);
    virtual void slot36();
    virtual void slot37(int);
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42(int, int, int, int, int, int, int);
    virtual void slot43();
};

extern "C" {
extern void func_02046838(...);
extern void func_0204dc90(...);
}

extern "C" void func_020623b8(char *param_1)
{
    if (*(unsigned char *)(param_1 + 0x98) != 0) {
        ((Object *)param_1)->slot42(10,0,1,0,1,-1,0);
        ((Object *)param_1)->slot35(0x1f);
    }
    else {
        ((Object *)param_1)->slot43();
        ((Object *)param_1)->slot35(1);
    }
    ((Object *)param_1)->slot37(0x3c);
    func_02046838(param_1,1);
    func_0204dc90(param_1,0x800);
}
