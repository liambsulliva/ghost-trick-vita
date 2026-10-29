void func_0200fa64(int param_1,int param_2,int param_3)

{
    int iVar1;

    if ((param_2 == ((int)0x0000ffffu)) || (param_3 == ((int)0x0000ffffu))) {
        *(unsigned int *)(param_1 + 0x10) = ((unsigned int)0xfbffffffu) & *(unsigned int *)(param_1 + 0x10);
        return;
    }
    *(unsigned int *)(param_1 + 0x10) = *(unsigned int *)(param_1 + 0x10) | 0x4000000;
    iVar1 = ((int)0x00000dacu);
    *(int *)(param_1 + ((int)0x00000dacu)) = (param_2 << 0x10) >> 4;
    *(int *)(param_1 + iVar1 + 4) = (param_3 << 0x10) >> 4;
    return;
}
