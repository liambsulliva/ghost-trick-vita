extern char *func_02008444(void);
extern void *func_02009900(void);
extern void *func_0202f624(int size);
extern void func_0202f7e8(void *list, void *object, int a, int b);
extern int func_020322d4(char *object);
extern void *func_020413fc(void *memory);
extern void func_02041518(void *object, int flag, int count);

void func_02040a14(int *param_1)
{
    int found;
    void *object;

    found = func_020322d4(func_02008444());
    object = func_0202f624(0xec);
    if (object != 0) {
        object = func_020413fc(object);
    }
    param_1[10] = (int)object;
    if (object != 0) {
        if (found == 0) {
            func_02041518(object, 0x8081, 8);
        } else {
            func_02041518(object, 0x8082, 8);
        }
        func_0202f7e8(func_02009900(), (void *)param_1[10], 0xa, 0);
    }
}
