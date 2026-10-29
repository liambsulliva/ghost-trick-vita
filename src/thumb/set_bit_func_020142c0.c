void func_020142c0(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 16);
    *slot = (*slot & 0xfffeffffu) | ((value & 1) << 16);
}
