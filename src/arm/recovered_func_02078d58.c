typedef int code();

void func_02078d58(unsigned int *param_1)

{
    if (param_1 == (unsigned int *)0x0) {
        return;
    }
    do {
        (*(code *)*param_1)(param_1[1]);
        param_1 = (unsigned int *)param_1[3];
    } while (param_1 != (unsigned int *)0x0);
    return;
}
