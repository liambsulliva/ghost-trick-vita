typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_02027c98();

void func_02010300(int param_1,int param_2)

{
    int iVar1;
    int iVar2;
    int *piVar3;

    iVar1 = ((int)0x0000104cu);
    if (*(int *)(param_1 + ((int)0x0000104cu)) != 0) {
        func_02007bd8();
        iVar2 = func_02007b70();
        if (iVar2 != 0) {
            func_02007bd8();
            piVar3 = (int *)func_02007b70();
            (**(code **)(*piVar3 + 0x10))(piVar3,*(unsigned int *)(param_1 + iVar1));
            *(unsigned int *)(param_1 + iVar1) = 0;
        }
    }
    func_02027c98(0,1);
    if (param_2 != 0) {
        *(int *)(param_1 + ((int)0x0000104cu)) = param_2;
        func_02027c98(param_2,1);
    }
    return;
}
