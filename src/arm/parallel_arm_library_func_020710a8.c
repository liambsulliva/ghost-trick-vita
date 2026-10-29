extern char data_020a24b8;
extern char data_020d0e64;
extern int func_02071c00();

void func_020710a8(void)

{
    unsigned short uVar1;
    int iVar2;
    int iVar3;

    iVar3 = func_02071c00();
    iVar2 = ((int)&data_020d0e64);
    uVar1 = ((unsigned short *)&data_020a24b8)[iVar3 >> 4];
    *(int *)(((int)&data_020d0e64) + 0xc) = iVar3;
    *(unsigned int *)(iVar2 + 8) = (unsigned int)uVar1 << 0xc;
    return;
}
