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
};

extern "C" {
extern int data_020af820;
extern int data_020cfc14;
extern void func_02062e50(...);
extern int func_02061cd8(...);
extern void func_020153d8(...);
extern int func_02009900(...);
extern void func_0202f7e8(...);
}

extern "C" char *func_020633fc(char *param_1)
{
    func_02062e50(param_1);
    *(int *)param_1 = (int)&data_020af820;
    *(int *)(param_1 + 0x9c) = 0;
    *(signed char *)(param_1 + 0xa0) = 0x1f;
    *(Object **)(param_1 + 0x9c) = (Object *)func_02061cd8(&data_020cfc14);
    (*(Object **)(param_1 + 0x9c))->slot37(0x3d);
    (*(Object **)(param_1 + 0x9c))->slot35(*(signed char *)(param_1 + 0xa0));
    func_020153d8(*(Object **)(param_1 + 0x9c),param_1);
    func_0202f7e8(func_02009900(),*(Object **)(param_1 + 0x9c),7,0);
    return param_1;
}
