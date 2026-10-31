extern int func_02047914();

void func_02047e40(int param_1, unsigned char *param_2)
{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_02047914(param_2);
    if (*(short *)(param_1 + 0xdc) == iVar1) {
        *(unsigned char *)(param_1 + 0xde) = 1;
        return;
    }
    for (uVar2 = 0; uVar2 < 6; uVar2 = uVar2 + 1) {
        if (func_02047914(param_2) == *(short *)(param_1 + uVar2 * 2 + 0x100)) {
            *(unsigned char *)(param_1 + uVar2 + 0x10c) = 1;
            return;
        }
    }
    return;
}
