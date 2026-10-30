extern char data_020abd14;
extern void *func_0202f524(void *object);
extern void *func_02051a2c(void *object);
extern void func_02051264(void *object, int value);
char *func_02051984(char *object)
{
    func_0202f524(object);
    *(void **)object = &data_020abd14;
    func_02051a2c(object + 0x28);
    *(int *)(object + 0x80) = 0;
    *(int *)(object + 0x7c) = 0;
    *(char **)(object + 0x58) = object;
    func_02051264(object + 0x5c, 0x20);
    return object;
}
