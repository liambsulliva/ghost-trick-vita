void func_0205eaec(int param_1,unsigned char param_2)

{
    int iVar1;

    iVar1 = ((int)0x00000711u);
    *(unsigned char *)(param_1 + ((int)0x00000711u)) = param_2;
    if (*(char *)(param_1 + iVar1) != '\0') {
        *(unsigned short *)(param_1 + iVar1 + 0xb) = 0;
        *(unsigned int *)(param_1 + iVar1 + 0xf) = 1;
        *(unsigned short *)(param_1 + iVar1 + 9) = 6;
        *(unsigned short *)(param_1 + iVar1 + 0xd) = 6;
    }
    return;
}
