typedef int code();

void func_ov004_020d6bbc(int param_1, int param_2)

{
    int *piVar1;

    piVar1 = *(int **)(param_1 + 0xaa4);
    (**(code **)(*piVar1 + 0x10))(piVar1, param_2);
}
