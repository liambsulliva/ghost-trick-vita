void func_0200cb88(char *object)
{
    int value;
    value = *(int *)(object + 12);
    if (value != 0) {
        *(int *)(object + 12) = value - 1;
    }
}
