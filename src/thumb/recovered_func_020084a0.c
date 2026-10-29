extern int func_020082f8();
extern int func_02059204();

void func_020084a0(int param_1,int param_2)

{
    int iVar1;

    iVar1 = func_020082f8(*(unsigned int *)(param_1 + 0x14),((unsigned int)0x020cf898u));
    if (iVar1 != 0) {
        func_02059204(iVar1,0,(int)*(short *)(param_2 + 2) << 0xc,(int)*(short *)(param_2 + 4) << 0xc);
    }
    return;
}
