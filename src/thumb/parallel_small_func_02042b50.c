extern void func_0200ca28(void *object);
extern void *func_02031254(int size);
extern void *func_02042718(void *object);
extern char data_020a7e70;

char *func_02042b50(char *object)
{
    void *value;

    func_0200ca28(object);
    *(int *)object = (int)&data_020a7e70;
    value = func_02031254(0x54);
    if (value != 0) {
        value = func_02042718(value);
    }
    *(void **)(object + 0x14) = value;
    return object;
}
