typedef int code();
extern int func_020169a4();

void func_0201695c(int param_1,unsigned int param_2,unsigned int param_3)

{
    int *piVar1;

    piVar1 = (int *)func_020169a4(param_1,6);
    *(int **)(param_1 + 4) = piVar1;
    if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 0xa0))(piVar1,param_2,param_3);
    }
    return;
}
