typedef int code();
extern int func_02016bec();

void func_02016bc4(int param_1,unsigned int param_2)

{
    int *piVar1;

    piVar1 = (int *)func_02016bec(param_1,0);
    *(int **)(param_1 + 4) = piVar1;
    if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 0xb4))(piVar1,param_2,0,0xac);
    }
    return;
}
