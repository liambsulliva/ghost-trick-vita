extern void func_0202fbcc(void *iterator, int kind);
extern int func_0200eac4(void *iterator);
extern void *func_0200eab4(void *iterator);
extern int func_0200fe48(void *object);
extern int func_02008828(void *object, void *kind);
extern void *func_0200eab8(void *iterator);
extern int func_0200daac(void *object);
extern int func_020790c8(int object, int value);
extern void func_0200eabc(void *iterator, int value);
extern char data_020cee88;

void *func_0205176c(int param_1)
{
    unsigned int iterator[4];
    int zero;
    void *item;

    func_0202fbcc(iterator, 5);
    zero = 0;
    while (func_0200eac4(iterator) != 0) {
        if (func_0200fe48(func_0200eab4(iterator)) != 0) {
            func_0200eabc(iterator, zero);
            continue;
        }
        if (func_02008828(func_0200eab4(iterator), &data_020cee88) != 0) {
            item = func_0200eab8(iterator);
            if (func_020790c8(param_1, func_0200daac(item)) == 0) {
                return item;
            }
        }
        func_0200eabc(iterator, 0);
    }
    return 0;
}
