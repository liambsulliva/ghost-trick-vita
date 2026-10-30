typedef void code();
extern void *func_02007bd8();
extern int func_02007b70();
extern int func_02027c98();

void func_0200fcc0(int param_1,unsigned int param_2)

{
    int iVar1;
    int *piVar2;
    int iVar3;
    int iVar4;

    if (param_2 < 4) {
        if (param_2 != 3) {
            iVar4 = param_2 * 4;
            iVar3 = param_1 + ((int)0x00001048u);
            if (*(int *)(iVar3 + iVar4) != 0) {
                iVar1 = func_02007b70(func_02007bd8());
                if (iVar1 != 0) {
                    piVar2 = (int *)func_02007b70(func_02007bd8());
                    ((code *)*(int *)(*piVar2 + 0x10))(piVar2,*(int *)(iVar3 + iVar4));
                    *(int *)(iVar3 + iVar4) = 0;
                }
            }
        }
        else {
            *(int *)(param_1 + param_2 * 4 + ((int)0x00001048u)) = 0;
        }
        func_02027c98(0,param_2);
    }
    return;
}
