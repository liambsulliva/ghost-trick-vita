extern int func_02076e58();
extern int func_02076ed0();
extern void func_02076f74();
extern int func_020769a0();
extern void func_02076b60(int zero);

void func_02076d68(int param_1)
{
    int iVar1;
    int iVar2;
    int iVar3;
    int iVar4;

    iVar1 = func_02076e58();
    if (iVar1 >= param_1) {
        return;
    }
    iVar2 = func_02076ed0();
    if (iVar1 + iVar2 >= param_1) {
        iVar4 = 0;
        do {
            iVar3 = func_020769a0(iVar4);
        } while (iVar3 != 0);
        iVar1 = func_02076e58();
        if (iVar1 >= param_1) {
            return;
        }
    }
    else {
        func_02076b60(1);
    }
    func_02076f74();
    iVar4 = 1;
    do {
        func_020769a0(iVar4);
        iVar1 = func_02076e58();
    } while (iVar1 < param_1);
    return;
}
