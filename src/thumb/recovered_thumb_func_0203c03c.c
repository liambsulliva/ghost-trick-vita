extern char data_0209a938;
extern char data_0209a9ec;
extern char data_0209a9f4;
extern char data_0209a9fc;
extern char data_0209aa04;
extern char data_0209aa18;
extern char data_0209aa28;
extern char data_0209aa30;
extern char data_020cd7d0;
extern char data_020cde4c;
extern int func_02008444();
extern int func_0200da04();
extern int func_02010384();
extern int func_0201123c();
extern int func_02011264();
extern int func_0201192c();
extern int func_02013514();
extern int func_020135b8();
extern int func_02022f2c();
extern int func_02022f34();
extern int func_02022fc0();
extern int func_0203bfbc();
extern int func_020790c8();

void func_0203c03c(unsigned int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02013514(param_3);
    iVar2 = func_020790c8(uVar1,((unsigned int)&data_0209a9ec));
    if (iVar2 == 0) {
        uVar1 = func_02011264();
        uVar1 = func_0201123c(uVar1,0);
        iVar2 = func_02022fc0(uVar1,((unsigned int)&data_020cde4c));
        if (iVar2 != 0) {
            func_0203bfbc();
            return;
        }
    }
    else {
        iVar2 = func_020790c8(uVar1,((unsigned int)&data_0209a9f4));
        if (iVar2 == 0) {
            func_0200da04(((unsigned int)&data_0209a9fc),((unsigned int)&data_0209aa04),((unsigned int)&data_0209aa18),((unsigned int)&data_0209a938));
            uVar1 = func_02011264();
            uVar1 = func_0201123c(uVar1,0);
            func_02022f2c(uVar1,((unsigned int)&data_020cd7d0),0);
            uVar1 = func_02008444();
            func_0201192c(uVar1,2);
            return;
        }
        iVar2 = func_020790c8(uVar1,((unsigned int)&data_0209aa28));
        if (iVar2 == 0) {
            uVar1 = func_02008444();
            func_020135b8(uVar1,1);
            uVar1 = func_02011264();
            uVar1 = func_0201123c(uVar1,0);
            func_02022f34(uVar1,((unsigned int)&data_0209aa30),3);
            uVar1 = func_02008444();
            func_0201192c(uVar1,0x15);
            uVar1 = func_02008444();
            func_02010384(uVar1,1);
        }
    }
    return;
}
