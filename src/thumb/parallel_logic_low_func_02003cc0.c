struct State {
    unsigned int before[4];
    int data;
};
extern struct State data_020b18a0;
extern int func_02000d9c();

int func_02003cc0(void)
{
    unsigned char *puVar1;
    int iVar2;
    unsigned int uVar3;
    int iVar4;
    int iVar5;

    iVar5 = 0;
    uVar3 = 0;
    do {
        iVar4 = data_020b18a0.data + ((int)0x00001a7cu) + uVar3 * 8;
        iVar2 = *(int *)(data_020b18a0.data + ((int)0x00001580u));
        puVar1 = (unsigned char *)func_02000d9c(iVar2,uVar3,*(int *)(*(int *)(iVar2 + 0x98) + 0x18));
        if (puVar1 != (unsigned char *)0x0) {
            iVar5 = iVar5 + 1;
            *(unsigned char *)(iVar4 + 2) = *puVar1;
            *(unsigned short *)(iVar4 + 4) = *(unsigned short *)(puVar1 + 2);
            *(unsigned char *)(iVar4 + 6) = 0x7f;
        }
        uVar3 = uVar3 + 1;
    } while ((int)uVar3 < 0x20);
    return iVar5;
}
