typedef int code();
extern int func_0201c268();

void func_0204e45c(int param_1,unsigned int param_2)

{
    int *piVar1;

    func_0201c268();
    piVar1 = *(int **)(param_1 + 0x44);
    if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 0x38))(piVar1,param_2);
    }
    piVar1 = *(int **)(param_1 + 0x48);
    if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 0x38))(piVar1,param_2);
    }
    piVar1 = *(int **)(param_1 + 0x4c);
    if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 0x38))(piVar1,param_2);
    }
    piVar1 = *(int **)(param_1 + 0x50);
    if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 0x38))(piVar1,param_2);
    }
    piVar1 = *(int **)(param_1 + 0x54);
    if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 0x38))(piVar1,param_2);
    }
    piVar1 = *(int **)(param_1 + 0x58);
    if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 0x38))(piVar1,param_2);
    }
    return;
}
