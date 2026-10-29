typedef int code();

int func_ov004_020d6b80(int param_1, int param_2)

{
    int *piVar1;

    piVar1 = *(int **)(param_1 + 0xaa4);
    return (**(code **)(*piVar1 + 8))(piVar1, param_2, 0x020e45f4);
}
