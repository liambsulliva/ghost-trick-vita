void func_0200db3c(void *object, int value);

void func_0200d80c(char *object, int value)
{
    *(int *)(object + 4) = value;
    func_0200db3c(object, value);
}
