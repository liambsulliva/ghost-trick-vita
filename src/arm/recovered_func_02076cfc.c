extern int func_020769a0();
extern int func_02076e08();
extern int func_02076f74();

void func_02076cfc(unsigned int param_1)

{
    int iVar1;

    iVar1 = func_02076e08();
    if (iVar1 != 0) {
        return;
    }
    do {
        iVar1 = func_020769a0(0);
    } while (iVar1 != 0);
    iVar1 = func_02076e08(param_1);
    if (iVar1 != 0) {
        return;
    }
    func_02076f74();
    iVar1 = func_02076e08(param_1);
    if (iVar1 != 0) {
        return;
    }
    do {
        func_020769a0(1);
        iVar1 = func_02076e08(param_1);
    } while (iVar1 == 0);
    return;
}
