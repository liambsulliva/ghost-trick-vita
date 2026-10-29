typedef int code();

void func_020467fc(int param_1,unsigned int param_2)

{
    int *piVar1;
    unsigned int uVar2;

    uVar2 = 0;
    do {
        piVar1 = *(int **)(param_1 + uVar2 * 4 + 0x28);
        (**(code **)(*piVar1 + 0x84))(piVar1,param_2);
        uVar2 = uVar2 + 1;
    } while (uVar2 < 2);
    return;
}
