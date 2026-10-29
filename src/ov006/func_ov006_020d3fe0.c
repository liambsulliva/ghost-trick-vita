void func_ov006_020d3fe0(int param_1,unsigned char param_2,unsigned char param_3,unsigned int param_4)

{
    int iVar1;

    iVar1 = ((unsigned int)0x00000162u);
    *(unsigned char *)(param_1 + ((unsigned int)0x00000162u)) = param_2;
    *(unsigned int *)(param_1 + iVar1 + 0x12) = param_4;
    *(unsigned char *)(param_1 + iVar1 + 1) = param_3;
    return;
}
