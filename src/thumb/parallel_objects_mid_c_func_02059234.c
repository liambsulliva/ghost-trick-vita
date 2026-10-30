extern int func_02008350();
extern int func_020592c0();
extern int func_0205935c();

void func_02059234(unsigned char *param_1,unsigned int param_2)

{
    int iVar1;

    iVar1 = func_02008350(param_1);
    if (iVar1 != 0) {
        if ((param_2 & 1) == 0) {
            iVar1 = *(int *)(param_1 + 0xe4);
            if (iVar1 < 0) {
                iVar1 = func_020592c0(iVar1);
                iVar1 = *(int *)(param_1 + 0xb0) - iVar1 * 2;
            }
            else {
                iVar1 = func_020592c0(iVar1);
                iVar1 = *(int *)(param_1 + 0xb0) + iVar1 * 2;
            }
            *(int *)(param_1 + 0xb0) = iVar1;
            func_0205935c(param_1,0);
        }
    }
    else if ((param_2 & 1) != 0) {
        iVar1 = *(int *)(param_1 + 0xe4);
        if (iVar1 < 0) {
            iVar1 = func_020592c0(iVar1);
            iVar1 = *(int *)(param_1 + 0xb0) - iVar1 * 2;
        }
        else {
            iVar1 = func_020592c0(iVar1);
            iVar1 = *(int *)(param_1 + 0xb0) + iVar1 * 2;
        }
        *(int *)(param_1 + 0xb0) = iVar1;
        func_0205935c(param_1,1);
    }
    return;
}
