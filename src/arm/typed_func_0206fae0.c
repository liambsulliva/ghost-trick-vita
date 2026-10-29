void func_0206fae0(unsigned int param_1,int param_2,unsigned short param_3,int param_4,int param_5)

{
    unsigned int *puVar1;
    unsigned int uVar2;

    puVar1 = ((unsigned int *)0x04000350u);
    uVar2 = param_1 | param_2 << 0x10 | param_4 << 0x18;
    if (param_5 != 0) {
        uVar2 = uVar2 | 0x8000;
    }
    *((unsigned int *)0x04000350u) = uVar2;
    *(unsigned short *)(puVar1 + 1) = param_3;
    return;
}
