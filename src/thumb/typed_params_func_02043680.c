extern int func_02043644();
extern int func_02043664();
extern int func_020436b8();

void func_02043680(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    int iVar1;

    if (*(int *)(param_1 + 0x3c) != 0) {
        iVar1 = func_02043644(param_1,*(int *)(param_1 + 0x3c),param_3,param_4);
        func_020436b8(*(unsigned char **)(param_1 + 0x3c),(unsigned int)(iVar1 == 0));
    }
    if (*(int *)(param_1 + 0x40) != 0) {
        iVar1 = func_02043664(param_1);
        func_020436b8(*(unsigned char **)(param_1 + 0x40),(unsigned int)(iVar1 == 0));
    }
    return;
}
