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
    virtual void slot35();
    virtual void slot36();
    virtual void slot37(int);
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
extern int data_020b00a8;
extern void func_02063bc0(...);
extern void func_0205765c(...);
}

extern "C" char *func_02063e90(char *param_1)
{
    func_02063bc0(param_1);
    *(int *)param_1 = (int)&data_020b00a8;
    *(short *)(param_1 + 0xca) = -1;
    *(short *)(param_1 + 0xcc) = -1;
    ((Object *)param_1)->slot46(0x100000,0xc0000);
    func_0205765c(param_1,0,0,0x100000,0xc0000);
    ((Object *)param_1)->slot37(0x39);
    ((Object *)param_1)->slot33(0);
    *(unsigned char *)(param_1 + 0xc7) = 1;
    return param_1;
}
