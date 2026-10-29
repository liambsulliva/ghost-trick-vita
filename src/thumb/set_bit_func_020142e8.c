void func_020142e8(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 16);
    *slot = (*slot & 0xffdfffffu) | ((value & 1) << 21);
}
