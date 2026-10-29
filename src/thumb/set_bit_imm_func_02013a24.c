void func_02013a24(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 12);
    *slot = (*slot & 0xfffffffdu) | ((value & 1) << 1);
}
