void func_0201420c(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 12);
    *slot = (*slot & 0xfffffbffu) | ((value & 1) << 10);
}
