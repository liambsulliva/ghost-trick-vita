extern int func_02027c08();

void func_0204649c(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned short uVar1;

    *(unsigned int *)(param_1 + 0x168) = param_2;
    *(unsigned int *)(param_1 + 0x16c) = param_3;
    *(unsigned int *)(param_1 + 0x170) = param_4;
    uVar1 = func_02027c08(param_2);
    *(unsigned short *)(param_1 + 0x174) = uVar1;
    uVar1 = func_02027c08(*(unsigned int *)(param_1 + 0x16c));
    *(unsigned short *)(param_1 + 0x176) = uVar1;
    uVar1 = func_02027c08(*(unsigned int *)(param_1 + 0x170));
    *(unsigned short *)(param_1 + 0x178) = uVar1;
    *(unsigned int *)(param_1 + 0xd0) = 0;
    *(unsigned char *)(param_1 + 0x130) = *(unsigned char *)(param_1 + 0x130) | 4;
    return;
}
