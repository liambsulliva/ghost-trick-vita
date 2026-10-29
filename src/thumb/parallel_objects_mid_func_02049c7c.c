extern unsigned int * func_02047120();

unsigned int * func_02049c7c(unsigned int *param_1)

{
    int iVar1;
    unsigned int uVar2;

    func_02047120(param_1);
    uVar2 = 0;
    *param_1 = ((unsigned int)0x020a9504u);
    *(unsigned char *)(param_1 + 0x2e) = 0;
    param_1[0x2f] = 0xffffffff;
    param_1[0x30] = 0;
    param_1[0x31] = 0;
    param_1[0x32] = 0;
    param_1[0x42] = 0;
    *(unsigned char *)(param_1 + 0x43) = 0x1f;
    param_1[0x44] = 0;
    param_1[0x45] = 0;
    *(unsigned char *)(param_1 + 0x48) = 0;
    *(unsigned char *)((int)param_1 + 0x121) = 0;
    *(unsigned char *)((int)param_1 + 0x122) = 0;
    param_1[0x49] = 0x20;
    param_1[0x4a] = 1;
    param_1[0x4b] = 0x20;
    param_1[0x4c] = 1;
    do {
        iVar1 = uVar2 * 4;
        uVar2 = uVar2 + 1;
        *(unsigned int *)((int)param_1 + iVar1 + 0xcc) = 0;
    } while (uVar2 < 0xf);
    return param_1;
}
