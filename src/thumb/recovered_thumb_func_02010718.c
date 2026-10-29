extern int func_02014a8c();

void func_02010718(int param_1,int param_2)

{
    int iVar1;

    iVar1 = ((int)0x00000f18u);
    if (param_2 != 0) {
        func_02014a8c(param_1 + ((int)0x00000f04u),param_2,0,0);
        return;
    }
    if (*(int *)(param_1 + ((int)0x00000f18u)) != 0) {
        func_02014a8c(param_1 + ((int)0x00000f18u) + -0x14,*(int *)(param_1 + ((int)0x00000f18u)),0,0);
        *(unsigned int *)(param_1 + iVar1) = 0;
    }
    return;
}
