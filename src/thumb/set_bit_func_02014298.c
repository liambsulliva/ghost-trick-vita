void func_02014298(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 16);
    *slot = (*slot & 0xfffdffffu) | ((value & 1) << 17);
}
