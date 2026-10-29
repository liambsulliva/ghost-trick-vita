extern int func_02008444();
extern int func_02010b38();
extern int func_02022f2c();
extern int func_02031b20();

void func_0203267c(unsigned int param_1)

{
    int iVar1;
    unsigned int uVar2;

    func_02008444();
    iVar1 = func_02010b38();
    if (iVar1 == 0) {
        uVar2 = func_02031b20(param_1);
        func_02022f2c(uVar2,((unsigned int)0x020cd7b4u),0);
    }
    return;
}
