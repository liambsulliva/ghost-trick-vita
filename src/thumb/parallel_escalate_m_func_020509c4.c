extern int func_02023fdc();
extern int func_02027548();
extern int func_02050a18();
extern void func_02050a20();
extern void func_02050a3c();

int func_020509c4(unsigned char *param_1)
{
    if (param_1[88] != 0) {
        while (func_02023fdc(param_1 + 20) != 0) {
        }
        if (func_02027548(param_1 + 20) != 0 || func_02050a18(param_1 + 20) != 0) {
            func_02050a20(param_1 + 20);
            func_02050a3c(param_1 + 20);
            param_1[88] = 0;
        } else {
            return 1;
        }
    }
    return 0;
}
