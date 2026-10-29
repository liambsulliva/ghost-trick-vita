extern int func_02012154();

void func_02012128(int param_1)

{
    int iVar1;

    iVar1 = ((int)0x0000ffffu);
    if (*(char *)(param_1 + 5) == '\0') {
        *(unsigned char *)(param_1 + 4) = 0;
        func_02012154((unsigned char *)(param_1 + 6),iVar1,8);
        func_02012154((unsigned char *)(param_1 + 0xe),iVar1,8);
    }
    return;
}
