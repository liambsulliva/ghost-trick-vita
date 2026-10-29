extern int func_02008444();
extern int func_02009900();
extern int func_0201192c();
extern int func_02022f34();
extern int func_0202fa8c();
extern int func_02031b20();
extern int func_0203247c();

void func_02035dd4(unsigned int param_1)

{
    unsigned int uVar1;
    int iVar2;
    unsigned int uVar3;

    uVar1 = func_02009900();
    iVar2 = func_0202fa8c(uVar1,((unsigned int)0x020cf4d8u));
    if (iVar2 == 0) {
        uVar1 = func_02031b20(param_1);
        func_02022f34(uVar1,((unsigned int)0x0209a4d8u),0);
        uVar1 = func_02008444();
        func_02008444();
        uVar3 = func_0203247c();
        func_0201192c(uVar1,uVar3);
    }
    return;
}
