void func_0201aba0(unsigned int param_1,int param_2,unsigned int param_3,unsigned char param_4)

{
    unsigned char bVar1;
    unsigned char bVar2;

    bVar1 = *(unsigned char *)(param_2 + (param_3 >> 1));
    bVar2 = bVar1 & 0xf0;
    bVar1 = bVar1 & 0xf;
    if ((param_3 & 1) == 0) {
        bVar1 = param_4;
    }
    else {
        bVar2 = param_4 << 4;
    }
    *(unsigned char *)(param_2 + (param_3 >> 1)) = bVar1 | bVar2;
    return;
}
