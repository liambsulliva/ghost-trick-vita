extern int func_02022dd8();
extern void *func_02008444(void);
extern int func_02010d18();
extern int func_0201132c();
extern int func_02010e1c();
extern int func_02010e3c();
extern int func_02011454();
extern int func_02031b30();
extern int func_020322d4();
extern int func_02013a14();
extern int func_02010f0c();
extern int func_02010930();

void func_020368e4(unsigned char *param_1)
{
    param_1[5] = 4;
    *(unsigned int *)(param_1 + 0x20) = 0;
    *(unsigned int *)(param_1 + 0x24) = 0;
    *(unsigned int *)(param_1 + 0x28) = func_02022dd8(param_1);
    func_02010d18(func_02008444());
    if (func_0201132c(func_02008444()) != 0) {
        func_02010e3c(func_02008444());
    } else {
        func_02010e1c(func_02008444());
    }
    func_02011454(func_02008444());
    func_02031b30(func_02008444(), 1);
    if (func_020322d4(func_02008444()) != 0) {
        if (func_02013a14(func_02008444()) != 0) {
            func_02010f0c(func_02008444(), 1, 1);
        } else {
            func_02010f0c(func_02008444(), 1, 0);
        }
    }
    func_02010930(func_02008444());
}
