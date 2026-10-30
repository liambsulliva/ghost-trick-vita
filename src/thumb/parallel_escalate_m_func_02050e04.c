extern char data_020abcdc;
extern char data_0209c108;
extern void *func_02007aa8(void *object);
extern void *func_02050f9c(void *object);
char *func_02050e04(char *object)
{
    func_02007aa8(object);
    *(void **)object = &data_020abcdc;
    func_02050f9c(object + 4);
    *(int *)(object + 0x3c) = 0;
    *(void **)(object + 0x48) = &data_0209c108;
    *(unsigned char *)(object + 0x4c) = 0;
    *(int *)(object + 0x40) = 0;
    *(int *)(object + 0x44) = 0;
    *(char **)(object + 0x38) = object;
    return object;
}
