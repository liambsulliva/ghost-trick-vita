typedef int code();
extern int func_02008444();
extern int func_02049bdc();
extern int func_02049be8();
extern int func_02054ae0();

void func_02054a6c(int param_1)

{
    int iVar1;
    int iVar2;
    int iVar3;

    iVar2 = *(int *)(param_1 + 0x34);
    iVar3 = *(int *)(param_1 + 0x38);
    func_02008444();
    iVar1 = func_02054ae0();
    if (iVar1 != 0) {
        func_02008444();
        iVar1 = func_02049bdc();
        iVar2 = iVar2 + (iVar1 >> 0xc);
        func_02008444();
        iVar1 = func_02049be8();
        iVar3 = iVar3 + (iVar1 >> 0xc);
    }
    (**(code **)(**(int **)(param_1 + 0x2c) + 0x38))
                        (*(int **)(param_1 + 0x2c),(iVar2 + -0x78) * 0x1000);
    (**(code **)(**(int **)(param_1 + 0x28) + 0x38))(*(int **)(param_1 + 0x28),iVar2 << 0xc);
    (**(code **)(**(int **)(param_1 + 0x30) + 0x38))
                        (*(int **)(param_1 + 0x30),(iVar2 + 0x78) * 0x1000);
    iVar1 = (iVar3 + -0x4a) * 0x1000;
    (**(code **)(**(int **)(param_1 + 0x2c) + 0x3c))(*(int **)(param_1 + 0x2c),iVar1);
    (**(code **)(**(int **)(param_1 + 0x28) + 0x3c))(*(int **)(param_1 + 0x28),iVar1);
    (**(code **)(**(int **)(param_1 + 0x30) + 0x3c))(*(int **)(param_1 + 0x30),iVar1);
    return;
}
