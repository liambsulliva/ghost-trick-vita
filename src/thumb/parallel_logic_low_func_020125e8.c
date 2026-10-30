void func_020125e8(char *param_1)
{
    unsigned char bVar1;

    bVar1 = *(unsigned char *)(param_1 + 0x5d);
    if (bVar1 > *(unsigned char *)(param_1 + 0x5c)) {
        *(unsigned char *)(param_1 + 0x5c) = bVar1;
        *(unsigned int *)(param_1 + 0x60) |= 1 << bVar1;
    }
    return;
}
