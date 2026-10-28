int func_02009040(void *object, int *out, int size, int zero);

unsigned char func_0200902c(void *object)
{
    int value;
    return func_02009040(object, &value, 4, 0);
}
