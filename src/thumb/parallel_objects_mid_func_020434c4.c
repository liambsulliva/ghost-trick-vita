extern int func_02043500();
extern int func_020435a4();
extern void func_0204437c();
int func_020434c4(int param_1, int param_2, int param_3)
{
    if (param_2 != 0) {
        if (func_02043500(param_1) != 0) {
            func_0204437c(*(int *)(param_1 + 0x3c));
            *(unsigned char *)(param_1 + 0x10) = 2;
        }
        return 1;
    }
    if (param_3 != 0) {
        if (func_020435a4(param_1) != 0) {
            func_0204437c(*(int *)(param_1 + 0x40));
            *(unsigned char *)(param_1 + 0x10) = 2;
        }
        return 1;
    }
    return 0;
}
