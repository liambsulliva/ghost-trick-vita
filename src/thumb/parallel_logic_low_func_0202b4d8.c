extern int data_020ca1d8;
void func_0202b4d8(int param_1)

{
    int *piVar1;

    if (param_1 != 0) {
        *(unsigned int *)(param_1 + 0x27c) = 0;
        piVar1 = &data_020ca1d8;
        *(int *)(param_1 + 0x278) = *&data_020ca1d8;
        *(int *)(*piVar1 + 0x27c) = param_1;
        *piVar1 = param_1;
    }
    return;
}
