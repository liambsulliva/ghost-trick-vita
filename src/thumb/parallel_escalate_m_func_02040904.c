extern void *func_02008444(void);
extern int func_02010194(void *game);
extern int func_020330b4(void *game);
extern int func_020322d4(void *game);
extern int func_020409a8(int value);
extern void func_020415b0(int target, unsigned int id, int a, int b, int c, int d);
extern void func_02058a80(int *out, int obj);
extern unsigned short *func_02058ab8(int obj);

void func_02040904(int param_1)
{
    int pos1[3];
    int pos2[3];
    int obj1;
    int obj2;
    int value;
    unsigned short *first;
    unsigned short *second;
    unsigned short *tmp;

    obj1 = func_02010194(func_02008444());
    obj2 = func_020330b4(func_02008444());
    first = 0;
    second = 0;
    if (obj1 != 0) {
        first = func_02058ab8(obj1);
    }
    if (obj2 != 0) {
        second = func_02058ab8(obj2);
    }
    func_02058a80(pos1, obj1);
    obj1 = func_020409a8(pos1[0]);
    func_02058a80(pos2, obj2);
    if (obj1 > func_020409a8(pos2[0])) {
        tmp = first;
        first = second;
        second = tmp;
    }
    value = func_020322d4(func_02008444());
    if (first != 0 && *first != 0) {
        func_020415b0(*(int *)(param_1 + 0x28), *first, 8, 0, 0x2a, value);
    }
    if (obj2 != 0 && *second != 0) {
        func_020415b0(*(int *)(param_1 + 0x28), *second, 8, 0x80, 0x2a, value);
    }
}
