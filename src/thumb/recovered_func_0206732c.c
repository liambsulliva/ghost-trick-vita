extern int func_02008444();
extern int func_0202c394();
extern int func_0203435c();
extern int func_02066108();

void func_0206732c(unsigned int param_1)

{
    int iVar1;

    func_02008444();
    iVar1 = func_0202c394();
    if (iVar1 == 0) {
        func_02008444();
        iVar1 = func_0203435c();
        if (iVar1 == 0) {
            func_02066108(param_1);
        }
    }
    return;
}
