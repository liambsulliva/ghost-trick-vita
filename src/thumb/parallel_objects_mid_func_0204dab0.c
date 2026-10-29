typedef int code();
extern int func_02008378();
extern int func_020083b0();
extern int func_020083c0();
extern int func_0201f0a0();
extern int func_020349b0();
extern int func_020471ec();
extern int func_02047250();
extern int func_020632f0();

void func_0204dab0(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char auStack_20 [12];
    unsigned char auStack_2c [12];
    unsigned char auStack_38 [12];
    unsigned char auStack_44 [12];
    unsigned char auStack_50 [12];
    unsigned char auStack_5c [12];
    unsigned int uStack_14;

    uStack_14 = param_4;
    func_020471ec(auStack_44,param_1);
    func_020083b0(param_1 + 0xbc,auStack_44);
    func_02047250(auStack_50,param_1);
    func_020083b0(param_1 + 200,auStack_50);
    func_020083c0(auStack_20,param_1 + 0xbc,param_1 + 200);
    func_0201f0a0(auStack_2c,auStack_20);
    func_020349b0(auStack_5c,auStack_2c,*(unsigned int *)(param_1 + 0x38));
    func_02008378(auStack_38,param_1 + 0xbc,auStack_5c);
    func_020632f0(*(unsigned int *)(param_1 + 0xb8),param_1 + 0xbc,auStack_38,
                                *(int *)(param_1 + 0x34) >> 7,0x800);
    (**(code **)(**(int **)(param_1 + 0xb8) + 0x8c))
                        (*(int **)(param_1 + 0xb8),(int)(signed char)*(short *)(param_1 + 0xb6));
    return;
}
