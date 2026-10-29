extern int func_02027c08();

void func_02045930(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned short uVar1;

    *(unsigned int *)(param_1 + 0x130) = param_2;
    *(unsigned int *)(param_1 + 0x134) = param_3;
    *(unsigned int *)(param_1 + 0x138) = param_4;
    uVar1 = func_02027c08(param_2);
    *(unsigned short *)(param_1 + 0x13c) = uVar1;
    uVar1 = func_02027c08(*(unsigned int *)(param_1 + 0x134));
    *(unsigned short *)(param_1 + 0x13e) = uVar1;
    uVar1 = func_02027c08(*(unsigned int *)(param_1 + 0x138));
    *(unsigned short *)(param_1 + 0x140) = uVar1;
    return;
}
