extern int func_02008444();
extern int func_020108ec();
extern int func_02010edc();
extern int func_02022dd8();
extern int func_02034248();
extern int func_02034250();
extern int func_02034258();

void func_02034210(unsigned int param_1)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02008444();
    func_02010edc(uVar1,0xff);
    func_02008444();
    func_020108ec();
    iVar2 = func_02022dd8(param_1);
    if (iVar2 != 0) {
        func_02034248(iVar2,0);
        func_02034250(iVar2,0);
        func_02034258(iVar2,0);
    }
    return;
}
