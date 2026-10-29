void func_0200db40(void *object, int value);

void func_0200d818(char *object, int value)
{
    *(int *)(object + 8) = value;
    func_0200db40(object, value);
}
