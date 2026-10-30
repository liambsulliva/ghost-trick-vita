extern int func_0200902c();
extern int func_020095dc();

void func_0200959c(char *param_1, int param_2, int param_3, int param_4)
{
    int kind;
    int result;

    kind = func_0200902c(param_1);
    result = 1;
    if (kind == 1) {
        result = func_020095dc(param_1);
    }
    if (kind != 1) {
        result = 1;
    }
    if (result == 0) {
        param_1[6] = 1;
        return;
    }
    if (kind == 2) {
        param_1[6] = 2;
        return;
    }
    if (result != 0) {
        param_1[6] = 2;
        return;
    }
    param_1[6] = 2;
    return;
}
