typedef int code();

extern int *data_020bbc34;
extern char data_02084e68;
extern char data_02084e78;
extern int *func_02031254(int size);
extern int *func_0201e2cc(int *object);
extern int func_0201e2ec(int *storage, int size, int count, unsigned int buffer, unsigned int capacity);
extern void func_0200c530(int *object, char *name);

int func_0201f698(int size)
{
    int *object;

    if (data_020bbc34 == 0) {
        object = func_02031254(0x20);
        if (object != 0) {
            object = func_0201e2cc(object);
        }
        data_020bbc34 = object;
        func_0201e2ec(object, 0xc, 0x200, 0, 0);
        func_0200c530(data_020bbc34, &data_02084e68);
    }
    return (*(code **)(*data_020bbc34 + 8))(data_020bbc34, 0, &data_02084e78);
}
