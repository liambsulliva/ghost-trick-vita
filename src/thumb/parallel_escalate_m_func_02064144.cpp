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
    virtual void slot43();
    virtual void slot44();
    virtual void slot45();
    virtual void slot46(int, int);
};

extern "C" {
extern int data_020cf724;
extern Object *func_0205693c(...);
extern int func_02009900(...);
extern void func_0202f7e8(...);
}

extern "C" void func_02064144(Object **param_1)
{
    unsigned int i;

    ((Object *)param_1)->slot42(0x20,0,1,0,1,-1,0);
    ((Object *)param_1)->slot46(0x100000,0x20000);
    ((Object *)param_1)->slot13(0,0xfff70000,0);
    i = 0;
    do {
        param_1[i + 0x32] = func_0205693c(&data_020cf724);
        param_1[i + 0x32]->slot46(0x40000,0x2000);
        func_0202f7e8(func_02009900(),param_1[i + 0x32],7,0);
        i = i + 1;
    } while (i < 4);
}
