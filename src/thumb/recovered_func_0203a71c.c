void func_0203a71c(int param_1,int param_2)

{
    unsigned int uVar1;

    if (param_2 == 0) {
        uVar1 = 0xc000;
    }
    else {
        if (param_2 != 1) goto code_r0x0203a734;
        uVar1 = 0x12000;
    }
    *(unsigned int *)(param_1 + 0xec) = uVar1;
code_r0x0203a734:
    *(unsigned char *)(param_1 + 0x10) = 3;
    return;
}
