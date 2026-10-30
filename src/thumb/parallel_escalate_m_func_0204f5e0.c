extern char data_020ab708;
extern void *func_0202f524(void *object);
extern void *func_0200aca8(void *object);
char *func_0204f5e0(char *object)
{
    unsigned int i;
    func_0202f524(object);
    *(void **)object = &data_020ab708;
    *(signed char *)(object + 0x2e) = -1;
    func_0200aca8(object + 0x30);
    for (i = 0; i < 12; i++) {
        ((int *)(object + 0x48))[i] = 0;
    }
    return object;
}
