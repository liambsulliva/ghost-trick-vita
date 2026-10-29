void func_0200fe38(char *object, int value)
{
    *(int *)(object + 8) = (*(int *)(object + 8) & ~1) | (value & 1);
}
