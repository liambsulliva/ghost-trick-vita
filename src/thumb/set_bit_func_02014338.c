void func_02014338(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 16);
    *slot = (*slot & 0xfdffffffu) | ((value & 1) << 25);
}
