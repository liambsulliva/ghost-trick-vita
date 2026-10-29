extern char data_0209a2f8;
extern char data_0209a304;
extern char data_020cd96c;
extern int func_0201123c();
extern int func_02011264();
extern int func_02013514();
extern int func_02022fc0();
extern int func_02034da8();
extern int func_02034dc4();
extern int func_020790c8();

void func_02034d54(unsigned int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;
    unsigned int uVar2;
    int iVar3;
    int iVar4;

    uVar1 = func_02013514(param_3);
    uVar2 = func_02011264();
    uVar2 = func_0201123c(uVar2,0);
    iVar3 = func_02022fc0(uVar2,((unsigned int)&data_020cd96c));
    if (iVar3 != 0) {
        iVar4 = func_020790c8(uVar1,((unsigned int)&data_0209a2f8));
        if (iVar4 == 0) {
            func_02034da8(iVar3);
            return;
        }
        iVar4 = func_020790c8(uVar1,((unsigned int)&data_0209a304));
        if (iVar4 == 0) {
            func_02034dc4(iVar3);
        }
    }
    return;
}
