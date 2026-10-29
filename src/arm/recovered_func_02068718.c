typedef int code();

void func_02068718(int param_1,code *param_2,unsigned int param_3)

{
    int iVar1;
    int iVar2;

    iVar1 = *(int *)(param_1 + 0x2c);
    if (*(int *)(param_1 + 0x2c) == 0) {
        return;
    }
    do {
        iVar2 = *(int *)(iVar1 + 0xc);
        (*param_2)(iVar1 + 0x10,param_1,param_3);
        iVar1 = iVar2;
    } while (iVar2 != 0);
    return;
}
