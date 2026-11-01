typedef int code();

extern int func_02016bec(int a, int b);

void func_02016b40(int param_1, int param_2, int param_3, int param_4)
{
    int object;

    object = func_02016bec(param_1, 2);
    *(int *)(param_1 + 4) = object;
    if (object != 0) {
        (*(code **)(*(int *)object + 0xa4))(object, param_2, param_3, param_4);
    }
}
