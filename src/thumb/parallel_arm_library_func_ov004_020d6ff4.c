unsigned int func_ov004_020d3d2c(unsigned int);

unsigned int func_ov004_020d6ff4(int param_1, int param_2)

{
    int iVar1;
    unsigned int uVar2;

    if (param_2 == -1) {
        iVar1 = 0x9d4;
    }
    else if (param_2 == 0) {
        iVar1 = 0x9cc;
    }
    else {
        iVar1 = 0x9d0;
    }
    uVar2 = *(unsigned int *)(param_1 + iVar1);
    if (uVar2 != 0) {
        return func_ov004_020d3d2c(uVar2);
    }
    return 0;
}
