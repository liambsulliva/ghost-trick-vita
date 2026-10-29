void func_020522f4(void *object, int value);

void func_020522e8(char *object, int value)
{
    *(int *)(object + 48) = value;
    func_020522f4(object, value);
}
