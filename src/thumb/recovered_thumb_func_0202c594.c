unsigned int func_0202c594(int param_1,unsigned int param_2,int param_3,int param_4,int param_5)

{
    unsigned int uVar1;

    uVar1 = 0;
    do {
        if (*(char *)(param_3 + uVar1) != '\0') {
            *(int *)(param_1 + uVar1 * 0xc + 0xa0) = *(int *)(param_4 + uVar1 * 4) << 0xc;
            *(int *)(param_1 + uVar1 * 0xc + 0xa4) = *(int *)(param_5 + uVar1 * 4) << 0xc;
            *(unsigned char *)(param_1 + 4) = *(unsigned char *)(param_1 + 4) | 4;
        }
        uVar1 = uVar1 + 1;
    } while (uVar1 < 4);
    return 1;
}
