class ObjectTyped {
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
    virtual void slot33(int argument0);
};

extern "C" {
typedef void code(...);
typedef int icode(...);
extern char data_020afb30;
extern char data_0209de5c;
extern char data_020cfc4c;
extern void func_020569f0(...);
extern int func_02007bd8(...);
extern int *func_02007b70(...);
extern void func_0202fbb0(...);
extern int func_0200eac4(...);
extern int func_0200eab4(...);
extern int func_02008828(...);
extern int func_0200eab8(...);
extern void func_02062be8(...);
extern void func_0200eabc(...);

}

extern "C" {
int *func_020621a0(int *param_1)
{
    int iVar3;
    unsigned int uVar5;
    unsigned int uVar6;
    int *piVar2;
    int puVar1;
    int puVar4;
    int local[4];
    int zero;

    func_020569f0(param_1);
    *param_1 = (int)&data_020afb30;
    uVar5 = 0;
    param_1[0x24] = 0x1000;
    *(short *)(param_1 + 0x25) = 0x540;
    *(short *)((unsigned char *)param_1 + 0x96) = -1;
    *(unsigned char *)(param_1 + 0x26) = 1;
    *((unsigned char *)param_1 + 0x99) = 0;
    param_1[0x1f] = 0;
    *((unsigned char *)param_1 + 0x9a) = 0;
    func_02007bd8();
    piVar2 = (int *)func_02007b70();
    iVar3 = (*(icode **)(*piVar2 + 8))(piVar2, 0x108, (int)&data_0209de5c);
    param_1[0x3c] = iVar3;
        ((ObjectTyped *)param_1)->slot10();
    uVar6 = uVar5;
    do {
        *((unsigned char *)param_1 + uVar5 + 0x9b) = (unsigned char)uVar6;
        uVar5 = uVar5 + 1;
    } while (uVar5 < 4);
        ((ObjectTyped *)param_1)->slot33(0x560);
    uVar6 = 0;
    do {
        param_1[uVar6 + 0x2c] = 0;
        uVar6 = uVar6 + 1;
    } while (uVar6 < 0x10);
    func_0202fbb0(local);
    puVar1 = (int)&data_020cfc4c;
    zero = 0;
    while (func_0200eac4(local) != 0) {
        puVar4 = func_0200eab4(local);
        iVar3 = func_02008828(puVar4, puVar1);
        if (iVar3 != 0) {
            iVar3 = func_0200eab8(local);
            func_02062be8((int)param_1, iVar3);
            func_02062be8(iVar3, (int)param_1);
        }
        func_0200eabc(local, zero);
    }
    return param_1;
}
}
