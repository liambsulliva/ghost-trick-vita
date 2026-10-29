void func_020142d4(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 16);
    *slot = (*slot & 0xffefffffu) | ((value & 1) << 20);
}
