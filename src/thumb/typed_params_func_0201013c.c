extern int func_020589f8();

void func_0201013c(int param_1,int param_2)

{
    int iVar1;

    if (param_2 == 0) {
        if (*(int *)(param_1 + ((int)0x00000ef4u)) != 0) {
            func_020589f8(*(int *)(param_1 + ((int)0x00000ef4u)),0);
        }
        *(unsigned int *)(param_1 + ((int)0x00000ef4u)) = 0;
        return;
    }
    iVar1 = *(int *)(param_1 + ((int)0x00000ef4u));
    if (iVar1 != param_2) {
        if (iVar1 != 0) {
            func_020589f8(iVar1,0);
        }
        *(int *)(param_1 + ((int)0x00000ef4u)) = param_2;
        func_020589f8(param_2,1);
    }
    return;
}
