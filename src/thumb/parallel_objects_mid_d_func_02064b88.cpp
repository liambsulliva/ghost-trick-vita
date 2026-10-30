class ObjectSlots {
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
    virtual void slot21(int argument0, int argument1);
};

extern "C" {
extern int func_0207b3dc(...);
extern int func_0207bd24(...);
extern int func_0207bf54(...);
extern int func_0207ba84(...);
extern int func_0207b424(...);
extern int func_02064854(...);
}

extern "C" {
void func_02064b88(int *param_1)
{
    unsigned int uVar1;
    int iVar2;
    int iVar3;
    int iVar4;

    iVar4 = param_1[0xc];
    param_1[0x16] = param_1[0x16] + iVar4;
    if (*(char *)((int)param_1 + 0x7a) == 0) {
        iVar2 = *(unsigned char *)((int)param_1 + 0x79) + 1;
        if (iVar2 * 0xc0 > 0) {
            uVar1 = func_0207b3dc(iVar2 * 0xc0000);
            func_0207bd24(0x3f000000,uVar1);
        }
        else {
            uVar1 = func_0207b3dc(iVar2 * 0xc0000);
            func_0207bf54(uVar1,0x3f000000);
        }
        iVar2 = func_0207ba84();
        iVar3 = param_1[0x16];
        if (iVar3 > iVar2) {
            iVar3 = iVar3 - iVar2;
            param_1[0x16] = iVar2;
            iVar4 = iVar4 - iVar3;
            *(char *)((int)param_1 + 0x79) = *(char *)((int)param_1 + 0x79) + 1;
        }
    }
    if (param_1[0x17] != 0) {
        uVar1 = func_0207b424(param_1[0x17] << 0xc);
        func_0207bd24(0x3f000000,uVar1);
    }
    else {
        uVar1 = func_0207b424(param_1[0x17] << 0xc);
        func_0207bf54(uVar1,0x3f000000);
    }
    iVar2 = func_0207ba84();
    if (param_1[0x16] > iVar2) {
        iVar3 = param_1[0x16] - iVar2;
        iVar4 = iVar4 - iVar3;
        param_1[0x16] = iVar2;
    }
    iVar2 = func_02064854(param_1[0x16]);
    param_1[0x15] = iVar2;
    ((ObjectSlots *)param_1)->slot21(param_1[0x1a] + iVar4,0);
    ((ObjectSlots *)param_1)->slot21(param_1[0x1b] + iVar4,1);
    param_1[0xc] = param_1[0xd];
    return;
}
}
