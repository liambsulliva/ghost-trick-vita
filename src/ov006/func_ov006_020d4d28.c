typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_02042b20();

void func_ov006_020d4d28(int param_1)

{
    int iVar1;
    int *piVar2;

    func_02042b20(*(unsigned int *)(param_1 + 0xcc));
    if (*(int *)(param_1 + 200) != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,*(unsigned int *)(param_1 + 200));
            *(unsigned int *)(param_1 + 200) = 0;
        }
    }
    return;
}
