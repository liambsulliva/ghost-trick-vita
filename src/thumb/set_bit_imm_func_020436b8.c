void func_020436b8(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 120);
    *slot = (*slot & 0xfffffffdu) | ((value & 1) << 1);
}
