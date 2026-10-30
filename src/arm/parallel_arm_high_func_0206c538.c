typedef struct {
    int n;
    int x;
} Pair;

extern int func_0206b9dc(int *obj, int code, int flag, int arg);

int func_0206c538(int *param_1, Pair pair, ...)
{
    int ret;
    int local[2];

    ret = 0;
    if (pair.n != 0) {
        param_1[2] = pair.n;
        param_1[4] = (int)local;
        local[0] = pair.x;
        local[1] = 0;
        ret = func_0206b9dc(param_1, 6, 1, pair.x);
    }
    return ret;
}
