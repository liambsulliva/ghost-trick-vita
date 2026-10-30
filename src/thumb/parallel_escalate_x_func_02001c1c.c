extern char data_020b18a0;
extern int func_02000d9c();

int func_02001c1c(unsigned int param_1)
{
    int iVar1;
    unsigned int *puVar2;
    int iVar3;
    int node;

    iVar3 = ((int)&data_020b18a0);
    iVar1 = *(int *)(*(int *)(((int)&data_020b18a0) + 0x10) + 0x1580);
    puVar2 = (unsigned int *)func_02000d9c(iVar1, param_1, *(int *)(*(int *)(iVar1 + 0x98) + 0x14));
    if (puVar2 == (unsigned int *)0x0) {
        return -5;
    }
    node = (*puVar2 & 0xffffff) * 0x10;
    node = (int)((char *)*(int *)(*(int *)(*(int *)(iVar3 + 0x10) + 0x1580) + 0x90) + node);
    if (*(int *)(node + 0x14) == 0) {
        return *(int *)(node + 0x10) + 0x20;
    }
    return 0;
}
