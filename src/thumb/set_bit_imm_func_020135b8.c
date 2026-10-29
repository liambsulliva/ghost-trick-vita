void func_020135b8(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 8);
    *slot = (*slot & 0xffffffbfu) | ((value & 1) << 6);
}
