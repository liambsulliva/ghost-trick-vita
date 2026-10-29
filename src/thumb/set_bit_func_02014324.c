void func_02014324(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 16);
    *slot = (*slot & 0xfeffffffu) | ((value & 1) << 24);
}
