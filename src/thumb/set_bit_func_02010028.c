void func_02010028(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 12);
    *slot = (*slot & 0xefffffffu) | ((value & 1) << 28);
}
