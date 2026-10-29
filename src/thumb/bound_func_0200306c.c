extern char data_020b18a0;
unsigned int func_0200306c(int param_1)

{
    unsigned int uVar1;
    unsigned int uVar2;

    uVar1 = 0;
    uVar2 = 0;
    do {
        if (param_1 == *(short *)(*(int *)(((int)&data_020b18a0) + 0x10) + uVar2 * 0x4c + ((int)0x000015beu))) {
            uVar1 = uVar1 | 1 << uVar2 & 0xffffU;
        }
        uVar2 = uVar2 + 1 & 0xff;
    } while (uVar2 < 0x10);
    return uVar1;
}
