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
    virtual void slot42(int, int, int, int, int, int, int);
};

extern "C" {
extern int func_02008444(...);
extern int func_0205b020(...);
extern int func_0205b3a4(...);
extern int func_0205bbb0(...);
extern int func_02027424(...);
}

extern "C" void func_0205bb40(char *param_1)
{
    int value;
    int found;
    signed char index;

    value = 0;
    index = *(signed char *)(param_1 + 0x9e);
    if (index != -1) {
        found = func_0205b020(func_02008444(),index);
    }
    else {
        found = func_0205b3a4(func_02008444(),*(signed char *)(param_1 + 0x9d));
    }
    if (found != 0) {
        value = func_0205bbb0(found);
    }
    if (func_02027424(value) != 0) {
        (*(Object **)(param_1 + 0x2c))->slot42(0xc4,0,1,0,1,-1,0);
    }
    else {
        (*(Object **)(param_1 + 0x2c))->slot42(0xc5,0,1,0,1,-1,0);
    }
}
