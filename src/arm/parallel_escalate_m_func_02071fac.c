typedef void (*BlockFunc)(void *context);

extern void func_02071ef0(void *context);
extern void func_020725c4(void *context);
extern int data_020d0ea0;
extern BlockFunc data_020b174c;

int func_02071fac(int flag)
{
    int old = data_020d0ea0;
    data_020d0ea0 = flag;
    if (flag) {
        data_020b174c = func_02071ef0;
    } else {
        data_020b174c = func_020725c4;
    }
    return old;
}
