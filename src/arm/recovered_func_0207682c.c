extern int func_02076aa0();
extern int func_02076b28();

void func_0207682c(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4,
                                    unsigned int param_5)

{
    int iVar1;

    iVar1 = func_02076aa0(1);
    if (iVar1 == 0) {
        return;
    }
    *(unsigned int *)(iVar1 + 4) = param_1;
    *(unsigned int *)(iVar1 + 8) = param_2;
    *(unsigned int *)(iVar1 + 0xc) = param_3;
    *(unsigned int *)(iVar1 + 0x10) = param_4;
    *(unsigned int *)(iVar1 + 0x14) = param_5;
    func_02076b28();
    return;
}
