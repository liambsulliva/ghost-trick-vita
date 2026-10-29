class Object020570e4_0 {
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
};

extern "C" {
extern char data_020adb20;
extern unsigned char * func_0202970c(...);
extern int func_02029708(...);
extern int func_0202c358(...);
extern int func_02057194(...);
extern int func_0205719c(...);
extern int func_020571ac(...);
extern int func_020571c0(...);
extern int func_0206f77c(...);
extern int func_0206f798(...);
extern int func_0207b694(...);

void func_020570e4(int *param_1)

{
    unsigned short uVar1;
    unsigned short sVar2;
    int iVar3;
    int iVar4;
    int iVar5;
    int negate;
    unsigned int uVar6;
    unsigned int uVar7;
    unsigned char auStack_40 [48];

    iVar3 = func_0202c358((int)param_1);
    if (iVar3 != 0) {
        if ((param_1[0x12] != 0) && (param_1[0x14] == -1)) {
            return;
        }
        iVar4 = func_02029708(func_0202970c());
        uVar1 = *(unsigned short *)((int)param_1 + 0x42);
        negate = -(iVar4 - (unsigned int)uVar1);
        iVar3 = (int)&data_020adb20;
        *(int *)(iVar3 + 0x24) = param_1[10] >> 7;
        iVar5 = func_0207b694(param_1[0xb],3);
        *(int *)(iVar3 + 0x28) = iVar5 >> 5;
        *(int *)(iVar3 + 0x2c) = negate;
        func_0206f77c((void *)0x020adb20u);
        sVar2 = *(unsigned short *)(param_1 + 0x10);
        if (sVar2 != 0) {
            uVar6 = func_0205719c(sVar2);
            uVar7 = func_020571ac(sVar2);
            func_02057194(auStack_40,uVar6,uVar7);
            func_0206f798(auStack_40);
        }
        if ((param_1[0xd] != 0x1000) || (param_1[0xe] != 0x1000)) {
            func_020571c0(param_1[0xd],param_1[0xe],0x1000);
        }
        ((Object020570e4_0 *)(param_1))->slot41();
        ((Object020570e4_0 *)(param_1))->slot52();
    }
    return;
}
}
