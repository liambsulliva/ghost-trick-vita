extern char data_020b18a0;
unsigned int func_02002f8c(unsigned int param_1,int param_2,int param_3,int param_4)

{
    short *psVar1;
    unsigned int uVar2;
    unsigned int uVar3;

    uVar2 = 0;
    uVar3 = 0;
    do {
        psVar1 = (short *)(*(int *)(((int)&data_020b18a0) + 0x10) + ((int)0x000015bcu) + uVar3 * 0x4c);
        if ((((*(unsigned char *)((int)psVar1 + 0x11) == param_1) || (param_1 == 0xffffffff)) &&
                (*(int *)(psVar1 + 2) == param_2)) &&
              ((*psVar1 == param_3 && ((psVar1[1] == param_4 || (param_4 == -1)))))) {
            uVar2 = uVar2 | 1 << uVar3 & 0xffffU;
        }
        uVar3 = uVar3 + 1 & 0xff;
    } while (uVar3 < 0x10);
    return uVar2;
}
