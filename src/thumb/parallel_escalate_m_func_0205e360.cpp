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
    virtual void slot33(int);
    virtual void slot34();
    virtual void slot35(int);
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
    virtual void slot46(int, int);
};

extern "C" {
extern int data_020cfad4;
extern void func_02056b18(...);
extern int func_0205bff4(...);
extern void func_0205e0dc(...);
extern void func_0205e0e8(...);
extern void func_020153d8(...);
extern int func_02009900(...);
extern void func_0202f7e8(...);
extern int func_0200847c(...);
extern void func_0202a0c0(...);
}

extern "C" void func_0205e360(unsigned char *param_1)
{
    int child;

    ((Object *)param_1)->slot46(0x100000,0xc0000);
    func_02056b18(param_1,0);
    ((Object *)param_1)->slot35(0x1f);
    ((Object *)param_1)->slot33(0x470);
    param_1[0xcf] = 0;
    param_1[0xcc] = 0;
    child = func_0205bff4(&data_020cfad4);
    *(int *)(param_1 + 0xc8) = child;
    if (child != 0) {
        func_0205e0dc(child,1,0);
        func_0205e0e8(*(int *)(param_1 + 0xc8),0);
        func_020153d8(*(int *)(param_1 + 0xc8),param_1);
        func_0202f7e8(func_02009900(),*(int *)(param_1 + 0xc8),7,0);
    }
    if (param_1[0xce] != 0) {
        func_0202a0c0(func_0200847c(),0xf);
    }
}
