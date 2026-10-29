void func_0200ee54(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 8);
    *slot = (*slot & 0xffffffefu) | ((value & 1) << 4);
}
