extern char data_020a9b4c;
extern void *func_02047120(void *object);
extern void func_02056b2c(void *object);
extern void *func_02056c8c(void *object);
extern void *func_0207dc50(int count, int size, int align, void *ctor, void *dtor);
char *func_02048d38(char *object)
{
    func_02047120(object);
    *(void **)object = &data_020a9b4c;
    *(int *)(object + 0xbc) = 0;
    *(int *)(object + 0xc0) = 0;
    *(unsigned char *)(object + 0xc4) = 0;
    *(void **)(object + 0xb8) = func_0207dc50(1, 0xc8, 8, func_02056b2c, func_02056c8c);
    return object;
}
