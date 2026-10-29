extern char data_02084cd8;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_0201bb98();
extern int func_0201bbbc();

void func_0201b890(int param_1)

{
    int iVar1;
    int *piVar2;
    unsigned int uVar3;

    if (*(int *)(param_1 + 0x68) != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,*(unsigned int *)(param_1 + 0x68));
            *(unsigned int *)(param_1 + 0x68) = 0;
        }
    }
    if (*(int *)(param_1 + 0x6c) != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,*(unsigned int *)(param_1 + 0x6c));
            *(unsigned int *)(param_1 + 0x6c) = 0;
        }
    }
    if (*(int *)(param_1 + 0x68) == 0) {
        func_02007bd8();
        piVar2 = (int *)func_02007b70();
        uVar3 = (**(code **)(*piVar2 + 8))(piVar2,*(unsigned int *)(param_1 + 0x78),((unsigned int)&data_02084cd8));
        *(unsigned int *)(param_1 + 0x68) = uVar3;
    }
    if (*(int *)(param_1 + 0x6c) == 0) {
        func_02007bd8();
        piVar2 = (int *)func_02007b70();
        uVar3 = (**(code **)(*piVar2 + 8))(piVar2,*(unsigned int *)(param_1 + 0x78),((unsigned int)&data_02084cd8));
        *(unsigned int *)(param_1 + 0x6c) = uVar3;
    }
    func_0201bb98(param_1);
    func_0201bbbc(param_1);
    return;
}
