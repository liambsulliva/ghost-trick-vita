extern int func_0200ba94();

void func_0200ba7c(int param_1,unsigned int *param_2)

{
    unsigned short uVar1;

    uVar1 = func_0200ba94(*param_2);
    *(unsigned short *)(param_1 + 0x2a) = uVar1;
    uVar1 = func_0200ba94(param_2[1]);
    *(unsigned short *)(param_1 + 0x2c) = uVar1;
    return;
}
