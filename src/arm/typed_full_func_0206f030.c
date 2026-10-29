void func_0206f030(unsigned int param_1,unsigned int param_2)

{
    unsigned short *puVar1;

    puVar1 = ((unsigned short *)0x04000280u);
    *((unsigned short *)0x04000280u) = 1;
    *(unsigned int *)(puVar1 + 8) = 0;
    *(unsigned int *)(puVar1 + 10) = param_1;
    *(unsigned int *)(puVar1 + 0xc) = param_2;
    *(unsigned int *)(puVar1 + 0xe) = 0;
    return;
}
