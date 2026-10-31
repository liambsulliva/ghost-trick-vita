extern int func_01ff80b8(void *object);
extern void *func_02008eb0(void);

int func_0205f13c(int param_1, int *param_2, int *param_3, int *param_4, int *param_5, short *param_6)
{
    int v;
    int half;
    int r;
    int d;

    if (*param_6 > 0) {
        half = *param_2 >> 1;
        r = func_01ff80b8(func_02008eb0());
        v = half + (r & half);
        d = v - *param_3;
    } else {
        r = func_01ff80b8(func_02008eb0());
        v = *param_2 & r;
        d = v - *param_3;
    }
    if (d >= 0) {
        d += *param_5;
    } else {
        d -= *param_5;
    }
    return d << (12 - *param_4);
}
