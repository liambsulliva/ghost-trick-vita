void func_02010384(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 8);
    *slot = (*slot & 0xfffffffdu) | ((value & 1) << 1);
}
