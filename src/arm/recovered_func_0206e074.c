extern int func_0206dd30();

unsigned int func_0206e074(int param_1,unsigned int param_2)

{
    int iVar1;
    unsigned int uVar2;

    uVar2 = 0;
    if ((*(int *)(*(int *)(param_1 + 8) + 0x24) == ((int)0x0209e3b8u)) &&
          (iVar1 = func_0206dd30(*(int *)(param_1 + 8),param_1,param_2), iVar1 == 0)) {
        uVar2 = 1;
    }
    return uVar2;
}
