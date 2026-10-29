extern int func_02075d14();
extern int func_020761d0();
extern int func_0207621c();
extern int func_02076fe4();

void func_02076f10(void)

{
    int iVar1;

    func_020761d0(7,((unsigned int)0x02076eecu));
    iVar1 = func_02076fe4();
    if (iVar1 == 0) {
        return;
    }
    iVar1 = func_0207621c(7,1);
    if (iVar1 != 0) {
        return;
    }
    do {
        func_02075d14(0x32);
        iVar1 = func_0207621c(7,1);
    } while (iVar1 == 0);
    return;
}
