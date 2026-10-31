typedef int code();

extern int func_0202f624();
extern int func_0205a7c8();
extern int func_0205765c();
extern int func_02009900();
extern int func_0202f7e8();

void func_0205b250(int param_1)
{
    int *piVar2;
    int iVar1;

    piVar2 = (int *)func_0202f624(0xcc);
    if (piVar2 != (int *)0x0) {
        piVar2 = (int *)func_0205a7c8(piVar2);
    }
    *(int **)(param_1 + 0x38) = piVar2;
    iVar1 = 0x40d;
    if (piVar2 != (int *)0x0) {
        (**(code **)(*piVar2 + 0xa8))(piVar2, iVar1, 0, 1, 0, 1, 0xffffffff, 0);
        (**(code **)(**(int **)(param_1 + 0x38) + 0x84))(*(int **)(param_1 + 0x38), iVar1 + 0x23);
        func_0205765c(*(int **)(param_1 + 0x38), 0, 0, 0xc0000, 0x18000);
        (**(code **)(**(int **)(param_1 + 0x38) + 0xb8))(*(int **)(param_1 + 0x38), 0xc0000, 0x18000);
        (**(code **)(**(int **)(param_1 + 0x38) + 0x34))(*(int **)(param_1 + 0x38), 0x20000, 0xfffac000, 0);
        (**(code **)(**(int **)(param_1 + 0x38) + 0x28))(*(int **)(param_1 + 0x38));
        func_0202f7e8(func_02009900(), *(int **)(param_1 + 0x38), 1, 0);
    }
    piVar2 = (int *)func_0202f624(0xcc);
    if (piVar2 != (int *)0x0) {
        piVar2 = (int *)func_0205a7c8(piVar2);
    }
    *(int **)(param_1 + 0x3c) = piVar2;
    if (piVar2 != (int *)0x0) {
        (**(code **)(*piVar2 + 0xa8))(piVar2, 0xc6, 0, 1, 0, 1, 0xffffffff, 0);
        (**(code **)(**(int **)(param_1 + 0x3c) + 0x84))(*(int **)(param_1 + 0x3c), 0x440);
        func_0205765c(*(int **)(param_1 + 0x3c), 0, 0, 0x100000, 0x18000);
        (**(code **)(**(int **)(param_1 + 0x3c) + 0xb8))(*(int **)(param_1 + 0x3c), 0x100000, 0x18000);
        (**(code **)(**(int **)(param_1 + 0x3c) + 0x34))(*(int **)(param_1 + 0x3c), 0, 0xfffac000, 0);
        (**(code **)(**(int **)(param_1 + 0x3c) + 0x28))(*(int **)(param_1 + 0x3c));
        func_0202f7e8(func_02009900(), *(int **)(param_1 + 0x3c), 1, 0);
    }
    return;
}
