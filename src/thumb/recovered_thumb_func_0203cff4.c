extern int func_0203bfb4();

void func_0203cff4(int param_1,unsigned char param_2)

{
    int iVar1;
    unsigned int uVar2;

    uVar2 = 0;
    *(unsigned char *)(param_1 + 5) = param_2;
    do {
        iVar1 = *(int *)(param_1 + uVar2 * 4 + 0x38);
        if (iVar1 != 0) {
            func_0203bfb4(iVar1,uVar2 == *(unsigned char *)(param_1 + 5));
        }
        uVar2 = uVar2 + 1 & 0xff;
    } while (uVar2 < 2);
    return;
}
