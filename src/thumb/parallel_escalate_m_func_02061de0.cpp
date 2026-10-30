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
};

extern "C" {
extern int data_020afa6c;
extern void func_020569f0(...);
}

extern "C" char *func_02061de0(char *param_1)
{
    func_020569f0(param_1);
    *(int *)param_1 = (int)&data_020afa6c;
    *(unsigned char *)(param_1 + 0x7c) = 1;
    *(unsigned short *)(param_1 + 0x7e) = 0x550;
    *(int *)(param_1 + 0x80) = 0xfff80000;
    *(int *)(param_1 + 0x84) = 0xfffa0000;
    *(int *)(param_1 + 0x88) = 0x80000;
    *(int *)(param_1 + 0x8c) = 0x60000;
    ((Object *)param_1)->slot33(0x550);
    ((Object *)param_1)->slot37(0x3e);
    return param_1;
}
