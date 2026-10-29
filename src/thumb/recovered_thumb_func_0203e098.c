extern char data_0209ac4c;
extern char data_0209ac58;
extern char data_0209ac68;
extern char data_0209ac74;
extern char data_0209ac80;
extern char data_0209ac88;
extern char data_0209ac90;
extern char data_020cdfd4;
extern int func_02008444();
extern int func_02010f0c();
extern int func_0201123c();
extern int func_02011264();
extern int func_02013514();
extern int func_02022fc0();
extern int func_0203e18c();
extern int func_020790c8();

void func_0203e098(unsigned int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;
    unsigned int uVar2;
    int iVar3;
    int iVar4;

    uVar1 = func_02013514(param_3);
    uVar2 = func_02011264();
    uVar2 = func_0201123c(uVar2,0);
    iVar3 = func_02022fc0(uVar2,((unsigned int)&data_020cdfd4));
    if (iVar3 != 0) {
        iVar4 = func_020790c8(uVar1,((unsigned int)&data_0209ac4c));
        if (iVar4 == 0) {
            func_0203e18c(iVar3,1,0x14);
            return;
        }
        iVar4 = func_020790c8(uVar1,((unsigned int)&data_0209ac58));
        if (iVar4 == 0) {
            func_0203e18c(iVar3,0,0x17);
            return;
        }
        iVar4 = func_020790c8(uVar1,((unsigned int)&data_0209ac68));
        if (iVar4 == 0) {
            func_0203e18c(iVar3,1,1);
            return;
        }
        iVar4 = func_020790c8(uVar1,((unsigned int)&data_0209ac74));
        if (iVar4 == 0) {
            func_0203e18c(iVar3,1,2);
            return;
        }
        iVar4 = func_020790c8(uVar1,((unsigned int)&data_0209ac80));
        if (iVar4 == 0) {
            func_0203e18c(iVar3,0,99);
            return;
        }
        iVar4 = func_020790c8(uVar1,((unsigned int)&data_0209ac88));
        if (iVar4 == 0) {
            func_0203e18c(iVar3,0,10);
            uVar1 = func_02008444();
            func_02010f0c(uVar1,2,3);
            return;
        }
        iVar4 = func_020790c8(uVar1,((unsigned int)&data_0209ac90));
        if (iVar4 == 0) {
            func_0203e18c(iVar3,0,0xff);
        }
    }
    return;
}
