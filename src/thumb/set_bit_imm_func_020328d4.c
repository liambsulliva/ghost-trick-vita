void func_020328d4(char *object, int value)
{
    unsigned int *slot;
    slot = (unsigned int *)(object + 8);
    *slot = (*slot & 0xffffffdfu) | ((value & 1) << 5);
}
