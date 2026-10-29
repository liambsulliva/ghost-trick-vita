void func_020276ec(char *object, int value)
{
    *(int *)(object + 20) = (*(int *)(object + 20) & ~1) | (value & 1);
}
