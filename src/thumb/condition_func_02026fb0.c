extern unsigned char func_02010b54();

void func_02026fb0(int param_1)

{
    unsigned char bVar1;

    bVar1 = func_02010b54(param_1);
    if (bVar1) {
        *(unsigned char *)(param_1 + ((int)0x00000141u)) = *(unsigned char *)(param_1 + ((int)0x00000141u)) & ~0x1 | 1;
    }
    return;
}
