void func_0201354c(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 12);
    *slot = (*slot & 0xfffffffbu) | ((value & 1) << 2);
}
