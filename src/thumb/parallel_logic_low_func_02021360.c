typedef int code();

int func_02021360(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    int iVar2;
    short sStack_18;
    unsigned char auStack_16 [10];
    unsigned int uStack_c;

    uStack_c = param_4;
    uVar1 = (**(code **)(**(int **)(param_1 + 4) + 0x2c))(*(int **)(param_1 + 4));
    iVar2 = uVar1 & 1;
    if (iVar2 != 0) {
        iVar2 = 2 - iVar2;
    }
    if (iVar2 != 0) {
        (**(code **)(**(int **)(param_1 + 4) + 0x18))(*(int **)(param_1 + 4),auStack_16,iVar2);
    }
    (**(code **)(**(int **)(param_1 + 4) + 0x18))(*(int **)(param_1 + 4),&sStack_18,2);
    return (int)sStack_18;
}
