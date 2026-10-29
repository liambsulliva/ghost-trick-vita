extern int func_02008444();
extern int func_0201192c();
extern int func_02031b04();

void func_02031b54(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_02008444();
    func_0201192c(uVar1,2);
    uVar1 = func_02008444();
    func_02031b04(uVar1,param_1 + 0x2c);
    uVar1 = func_02008444();
    func_02031b04(uVar1,param_1 + 0x30);
    uVar1 = func_02008444();
    func_02031b04(uVar1,param_1 + 0x34);
    return;
}
