typedef int code();
extern int func_020169a4();

void func_020166ac(int param_1,unsigned int param_2,unsigned int param_3)

{
    int *piVar1;

    piVar1 = (int *)func_020169a4(param_1,0);
    *(int **)(param_1 + 4) = piVar1;
    if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 0x2c))(piVar1,param_2,param_3);
    }
    return;
}
