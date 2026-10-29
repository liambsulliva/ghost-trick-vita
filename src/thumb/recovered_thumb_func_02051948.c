extern int func_02008828();
extern int func_02009900();
extern int func_0202f7e8();

void func_02051948(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    if ((*(int *)(param_1 + 0xb4) != 0) &&
          (iVar1 = func_02008828(*(int *)(param_1 + 0xb4),((unsigned int)0x020cf898u)), iVar1 != 0)) {
        *(unsigned int *)(*(int *)(param_1 + 0xb4) + 0xe0) = *(unsigned int *)(param_1 + 0xfc);
        uVar2 = func_02009900();
        func_0202f7e8(uVar2,*(unsigned int *)(param_1 + 0xb4),6,1);
    }
    return;
}
