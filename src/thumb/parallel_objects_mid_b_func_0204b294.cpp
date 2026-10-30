class Object0204b294 {
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
extern int func_0205765c(...);
extern int func_0200ee34(...);
extern unsigned int * func_02008eb0(...);
extern unsigned int func_01ff80b8(...);
extern unsigned long long func_0207b694(...);
}

extern "C" void func_0204b294(char *param_1)
{
    Object0204b294 *self;
    unsigned int *puVar1;
    unsigned int uVar2;
    int uVar3;
    int uVar4;

    self = (Object0204b294 *)param_1;
    self->slot42(0x18, 0, 1, 0, 1, -1, 0);
    self->slot46(0x20000, 0x20000);
    func_0205765c(param_1, 0, 0, 0x20000, 0x20000);
    func_0200ee34(param_1, 6, 0x16, 0x16);
    self->slot33(0xa00);
    *(int *)(param_1 + 0xd4) = 0x1f000;
    uVar3 = *(int *)(param_1 + 0xe4) >> 2;
    if (uVar3 > 0x1800) {
        uVar3 = 0x1800;
    }
    if (uVar3 < 0x600) {
        uVar3 = 0x600;
    }
    uVar4 = uVar3 >> 1;
    puVar1 = func_02008eb0();
    uVar2 = func_01ff80b8(puVar1);
    *(int *)(param_1 + 0xc8) = (int)((uVar2 & uVar3) - uVar4);
    puVar1 = func_02008eb0();
    uVar2 = func_01ff80b8(puVar1);
    *(int *)(param_1 + 0xcc) = (uVar3 + (int)(uVar2 & uVar4)) + uVar4;
    *(int *)(param_1 + 0xd0) = -(uVar3 >> 3);
    *(int *)(param_1 + 0xd8) = (int)func_0207b694(0x1000000, uVar3);
}
