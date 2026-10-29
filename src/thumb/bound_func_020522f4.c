extern char data_020cefc4;
void func_020522f4(int param_1)

{
    int iVar1;
    
    iVar1 = *(int *)(param_1 + 0x30) * 4;
    *(int *)(((unsigned int)&data_020cefc4) + iVar1) = *(int *)(((unsigned int)&data_020cefc4) + iVar1) + 1;
    return;
}
