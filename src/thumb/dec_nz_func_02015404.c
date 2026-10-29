void func_02015404(char *object)
{
    int value;
    value = *(int *)(object + 28);
    if (value != 0) {
        *(int *)(object + 28) = value - 1;
    }
}
