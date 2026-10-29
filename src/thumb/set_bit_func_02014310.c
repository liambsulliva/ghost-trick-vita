void func_02014310(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 16);
    *slot = (*slot & 0xf7ffffffu) | ((value & 1) << 27);
}
