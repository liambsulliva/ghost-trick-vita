void func_020142ac(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 16);
    *slot = (*slot & 0xfff7ffffu) | ((value & 1) << 19);
}
