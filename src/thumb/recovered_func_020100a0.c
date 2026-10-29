void func_020100a0(int param_1,int param_2)

{
    unsigned int uVar1;

    if (param_2 == 0) {
        uVar1 = ((unsigned int)0xfffff7ffu) & *(unsigned int *)(param_1 + 8);
    }
    else {
        uVar1 = *(unsigned int *)(param_1 + 8) | 0x800;
    }
    *(unsigned int *)(param_1 + 8) = uVar1;
    return;
}
