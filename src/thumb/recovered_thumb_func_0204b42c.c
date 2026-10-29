extern int func_02008444();
extern int func_02010b54();
extern int func_0202c394();
extern int func_0203435c();
extern int func_020570cc();

void func_0204b42c(int param_1)

{
    int iVar1;

    func_02008444();
    iVar1 = func_0202c394();
    if (iVar1 == 0) {
        func_02008444();
        iVar1 = func_0203435c();
        if (((iVar1 == 0) && (*(char *)(param_1 + 0xc) == '\x03')) &&
              (iVar1 = func_02010b54(param_1), iVar1 != 0)) {
            func_020570cc(param_1);
        }
    }
    return;
}
