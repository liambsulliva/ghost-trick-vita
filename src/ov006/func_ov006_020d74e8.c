extern int func_02008444();
extern int func_0201fc5c();
extern int func_0201ff10();

void func_ov006_020d74e8(unsigned int param_1,int param_2,int param_3)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02008444();
    iVar2 = func_0201ff10(uVar1,param_2 + 1);
    if (iVar2 != 0) {
        func_0201fc5c(iVar2,param_3 + 1);
    }
    return;
}
