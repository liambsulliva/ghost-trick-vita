void func_02046838(char *object, int value)
{
    *(int *)(object + 120) = (*(int *)(object + 120) & ~1) | (value & 1);
}
