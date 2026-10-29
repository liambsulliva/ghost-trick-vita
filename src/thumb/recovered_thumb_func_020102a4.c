typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_0200fcc0();

void func_020102a4(int param_1,int param_2)

{
    int iVar1;
    int *piVar2;
    int iVar3;

    iVar3 = ((int)0x00001044u);
    if (param_2 == 0) {
        if (*(int *)(param_1 + ((int)0x00001044u)) != 0) {
            func_02007bd8();
            iVar1 = func_02007b70();
            if (iVar1 != 0) {
                func_02007bd8();
                piVar2 = (int *)func_02007b70();
                (**(code **)(*piVar2 + 0x10))(piVar2,*(unsigned int *)(param_1 + iVar3));
                *(unsigned int *)(param_1 + iVar3) = 0;
            }
        }
        func_0200fcc0(param_1,1);
    }
    else {
        iVar3 = *(int *)(param_1 + ((int)0x00001044u));
        if (iVar3 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x14))(piVar2,iVar3);
        }
    }
    *(int *)(param_1 + ((int)0x00001044u)) = param_2;
    return;
}
