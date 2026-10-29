extern int func_02008444();
extern int func_0201003c();
extern int func_02011244();
extern int func_020151a0();
extern int func_02022f6c();

void func_02033908(unsigned int param_1)

{
    int iVar1;

    func_02008444();
    iVar1 = func_02011244();
    if (iVar1 == 0) {
        func_0201003c();
        iVar1 = func_020151a0();
        if (iVar1 == 0) {
            func_02022f6c(param_1);
        }
    }
    return;
}
