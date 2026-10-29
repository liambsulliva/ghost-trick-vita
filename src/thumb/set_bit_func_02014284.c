void func_02014284(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 12);
    *slot = (*slot & 0xbfffffffu) | ((value & 1) << 30);
}
