extern int func_02007aa8();

unsigned int * func_0201dcd8(unsigned int *param_1)

{
    int iVar1;

    func_02007aa8();
    iVar1 = ((int)0x00000aa8u);
    *param_1 = ((unsigned int)0x020a44f0u);
    *(unsigned int *)((int)param_1 + iVar1) = 0;
    *(unsigned int *)((int)param_1 + iVar1 + -0xdc) = 0;
    *(unsigned int *)((int)param_1 + iVar1 + -0xd8) = 0;
    *(unsigned int *)((int)param_1 + iVar1 + -0xd4) = 0;
    *(unsigned int *)((int)param_1 + iVar1 + -0x34) = 0;
    return param_1;
}
