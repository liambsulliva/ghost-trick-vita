int func_0204a150(int param_1, int *param_2, int *param_3, int *param_4, int *param_5, int *param_6, int *param_7)
{
    int ret;
    int n;
    int a;
    int base;

    ret = *param_7;
    n = *param_3;
    if (n > 0) {
        a = *param_2 - *param_4;
        if (a < 0) {
            a = 0;
        } else if (a > n) {
            a = n;
        }
        base = *param_5;
        ret = base + a * (*param_6 - base) / n;
    }
    return ret;
}
