void func_020142fc(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 16);
    *slot = (*slot & 0xff7fffffu) | ((value & 1) << 23);
}
