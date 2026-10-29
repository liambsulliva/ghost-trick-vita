extern int func_02047914();

void func_02057d40(int param_1,unsigned int param_2)

{
    int iVar1;

    iVar1 = func_02047914(param_2);
    if (*(short *)(param_1 + 0xe4) == iVar1) {
        *(unsigned char *)(param_1 + 0xe6) = 1;
    }
    return;
}
