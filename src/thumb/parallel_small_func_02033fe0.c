extern void func_02056ac8(int *object);
extern void *func_02031254(int size);
extern void *func_02042718(void *object);
extern char data_020a5a14;

int *func_02033fe0(int *object)
{
    void *value;

    func_02056ac8(object);
    *object = (int)&data_020a5a14;
    object[0x32] = 0;
    object[0x33] = 0;
    value = func_02031254(0x54);
    if (value != 0) {
        value = func_02042718(value);
    }
    object[0x33] = (int)value;
    return object;
}
