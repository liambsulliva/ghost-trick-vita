typedef void code();
void func_020386c4(int param_1)
{
    int *piVar1;
    piVar1 = *(int **)(param_1 + 8);
    if (piVar1 != (int *)0) {
        (*(code **)(*piVar1 + 0x84))(piVar1, 0x470);
    }
}
