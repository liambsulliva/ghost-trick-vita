void func_0202680c(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 20);
    *slot = (*slot & 0xfffffffdu) | ((value & 1) << 1);
}
