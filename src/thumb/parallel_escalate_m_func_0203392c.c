extern char *func_02008444(void);
extern int func_0201132c(char *object);
extern void func_02010e3c(char *object);
extern void func_02010e1c(char *object);
extern void func_02010f0c(char *object, int a, int b);
extern void func_02010930(char *object);
extern void func_02011080(char *object, unsigned char state);
extern void func_02010384(char *object, int value);

void func_0203392c(unsigned char *object)
{
    switch (object[0x21]) {
    case 3:
        if (func_0201132c(func_02008444()) != 0) {
            func_02010e3c(func_02008444());
        } else {
            func_02010e1c(func_02008444());
        }
        break;
    case 2:
        if (object[0x20] != 0) {
            func_02010f0c(func_02008444(), 1, 1);
        }
        break;
    }
    if (object[4] != 0) {
        func_02010930(func_02008444());
    }
    func_02011080(func_02008444(), object[0x21]);
    func_02010384(func_02008444(), 0);
}
