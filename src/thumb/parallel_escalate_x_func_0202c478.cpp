class View0202c478 {
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
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53(int value);
};

extern "C" {
void func_02008398(unsigned int *out, int x, int y, int z);
void func_020083b0(char *target, unsigned int *source);
void func_0202b284(char *target, unsigned short *source);
void func_0202c478(char *param_1, unsigned short *param_2, int param_3, int param_4);
}

void func_0202c478(char *param_1, unsigned short *param_2, int param_3, int param_4)
{
    unsigned int auStack_24[3];
    unsigned int auStack_18[3];
    unsigned int auStack_0c[3];
    unsigned int auStack_00[3];
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    unsigned int i;

    *(unsigned short **)(param_1 + 0x14) = param_2;
    *(unsigned short *)(param_1 + 0xc) = param_2[0];
    *(unsigned short *)(param_1 + 0x84) = param_2[1];
    iVar3 = *(int *)(param_2 + 2);
    *(int *)(param_1 + 0x88) = iVar3;
    iVar2 = *(int *)(param_2 + 4);
    *(int *)(param_1 + 0x8c) = iVar2;
    iVar1 = *(int *)(param_2 + 6);
    *(int *)(param_1 + 0x90) = iVar1;
    iVar4 = *(int *)(param_2 + 8);
    *(int *)(param_1 + 0x94) = iVar4;
    *(int *)(param_1 + 0x94) = iVar4 - iVar1;
    *(int *)(param_1 + 0x8c) = iVar2 - iVar3;
    *(int *)(param_1 + 0x88) = 0;
    *(int *)(param_1 + 0x90) = 0;
    func_02008398(auStack_24, *(int *)(param_1 + 0x88) << 12, *(int *)(param_1 + 0x90) << 12, 0x1000);
    func_020083b0(param_1 + 0xa0, auStack_24);
    func_02008398(auStack_18, *(int *)(param_1 + 0x88) << 12, *(int *)(param_1 + 0x94) << 12, 0x1000);
    func_020083b0(param_1 + 0xac, auStack_18);
    func_02008398(auStack_0c, *(int *)(param_1 + 0x8c) << 12, *(int *)(param_1 + 0x94) << 12, 0x1000);
    func_020083b0(param_1 + 0xb8, auStack_0c);
    func_02008398(auStack_00, *(int *)(param_1 + 0x8c) << 12, *(int *)(param_1 + 0x90) << 12, 0x1000);
    func_020083b0(param_1 + 0xc4, auStack_00);
    for (i = 0; i < *(unsigned short *)(param_1 + 0x84); i++) {
        *(unsigned short *)(param_1 + 0x86) = param_2[10];
        func_0202b284(param_1 + 0xd0, param_2 + 12);
        break;
    }
    ((View0202c478 *)(param_1 + 0xf4))->s53(*(int *)(param_1 + 0x80));
}
