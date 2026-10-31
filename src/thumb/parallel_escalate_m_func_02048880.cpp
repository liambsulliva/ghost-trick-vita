class Obj02048880 {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void m33(int a);
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void m42(int a, int b, int c, int d, int e, int f, int g);
};

extern "C" {
extern unsigned long long func_0207b8a0(unsigned int a, unsigned int b);
extern char data_0209b738;

void func_02048880(int *param_1)
{
    unsigned short uVar1;
    unsigned short *puVar2;
    unsigned short *puVar3;
    unsigned int uVar5;
    int iVar4;
    unsigned int uVar7;
    unsigned short auStack_28[8];
    unsigned long long uVar6;

    if (*(int *)((char *)param_1 + 0xb8) != 0) {
        uVar7 = 7;
        ((Obj02048880 *)param_1)->m42(7, 0, 1, 0, 1, -1, 0);
        puVar2 = auStack_28;
        puVar3 = (unsigned short *)&data_0209b738;
        do {
            uVar1 = *puVar3;
            puVar3 = puVar3 + 1;
            *puVar2 = uVar1;
            puVar2 = puVar2 + 1;
            uVar7 = uVar7 - 1;
        } while (uVar7 != 0);
        uVar5 = 0;
        do {
            iVar4 = uVar5 * 0x20;
            *(unsigned char *)(param_1[0x2e] + iVar4 + 1) = 0;
            *(unsigned int *)(param_1[0x2e] + iVar4 + 0x1c) = 0;
            *(unsigned int *)(param_1[0x2e] + iVar4 + 0x14) = 0x19a;
            uVar6 = func_0207b8a0(uVar5, 7);
            uVar5 = uVar5 + 1;
            *(unsigned short *)(param_1[0x2e] + iVar4 + 2) = auStack_28[(unsigned int)(uVar6 >> 32)];
        } while (uVar5 < 0x20);
        param_1[0x2f] = 0;
        *(unsigned char *)(param_1 + 0x30) = 0;
        ((Obj02048880 *)param_1)->m33(0xd00);
    }
    return;
}
}
