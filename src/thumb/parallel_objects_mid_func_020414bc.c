extern int func_0200b808();
void func_020414bc(unsigned char *param_1)
{
    int iVar1;
    unsigned int uVar2;

    if (param_1[0xc] == 3) {
        for (uVar2 = 0; uVar2 < param_1[0xbc]; uVar2 = uVar2 + 1) {
            iVar1 = *(int *)(param_1 + uVar2 * 4 + 0xc0);
            if (iVar1 != 0) {
                func_0200b808(iVar1);
            }
        }
    }
}
