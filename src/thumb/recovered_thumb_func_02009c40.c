extern int func_02009c74();

void func_02009c40(int param_1,unsigned int param_2,unsigned int param_3)

{
    int iVar1;
    unsigned short uVar2;

    iVar1 = ((int)0x0000aa28u);
    uVar2 = func_02009c74(param_2,*(unsigned short *)(param_1 + ((int)0x0000aa28u)) - 0x100);
    *(unsigned short *)(param_1 + iVar1 + 8) = uVar2;
    uVar2 = func_02009c74(param_3,*(unsigned short *)(param_1 + iVar1 + 2) - 0xc0);
    *(unsigned short *)(param_1 + iVar1 + 10) = uVar2;
    return;
}
