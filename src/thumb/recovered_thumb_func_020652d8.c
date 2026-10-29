extern int func_02008824();
extern int func_0200883c();
extern int func_02009c1c();
extern int func_0200cb5c();
extern int func_0201e888();
extern int func_0202f1bc();

void func_020652d8(void)

{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned int uVar3;

    func_0200883c();
    uVar1 = func_02008824();
    uVar1 = func_0201e888(uVar1,0,0x82,1,0);
    func_0200883c();
    func_02008824();
    uVar2 = func_0200cb5c();
    uVar3 = func_02009c1c();
    func_0202f1bc(uVar3,uVar1,0,uVar2,7);
    uVar3 = func_02009c1c();
    func_0202f1bc(uVar3,uVar1,0,uVar2,0x15);
    return;
}
