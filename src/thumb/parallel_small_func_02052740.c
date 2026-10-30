extern char data_020ac28c;
extern char *func_02008444(void);
extern void func_020297f0(char *object, int unused);
extern void func_02056c6c(char *object);

char *func_02052740(char *object)
{
    int handle;

    *(void **)object = &data_020ac28c;
    handle = *(int *)(object + 0x84);
    if (handle >= 0) {
        func_020297f0(func_02008444(), handle);
    }
    func_02056c6c(object);
    return object;
}
