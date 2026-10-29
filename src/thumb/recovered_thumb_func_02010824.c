extern int func_0202f64c();

void func_02010824(int param_1,int param_2)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = ((int)0x00000dfcu);
    func_0202f64c(param_1 + ((int)0x00000dfcu));
    uVar2 = 2;
    do {
        func_0202f64c(param_1 + iVar1 + uVar2 * 4);
        uVar2 = uVar2 + 1;
    } while (uVar2 < 8);
    if (param_2 != 0) {
        func_0202f64c(param_1 + 0xe00);
    }
    return;
}
