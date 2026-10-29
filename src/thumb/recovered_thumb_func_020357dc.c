extern char data_0209a414;
extern char data_0209a424;
extern char data_020cd7b4;
extern char data_020cd9d8;
extern char data_020cd9f4;
extern int func_02008444();
extern int func_02009900();
extern int func_02011080();
extern int func_0201123c();
extern int func_02011264();
extern int func_02013514();
extern int func_02022f2c();
extern int func_0202f7e8();
extern int func_0202fad4();
extern int func_0202fb48();
extern int func_02032644();
extern int func_02034f64();
extern int func_020790c8();

void func_020357dc(unsigned int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;
    int iVar2;
    unsigned int uVar3;

    uVar1 = func_02013514(param_3);
    iVar2 = func_020790c8(uVar1,((unsigned int)&data_0209a414));
    if (iVar2 == 0) {
        uVar3 = func_02009900();
        uVar1 = ((unsigned int)&data_020cd9f4);
        iVar2 = func_0202fb48(uVar3,((unsigned int)&data_020cd9f4),0xffffffff);
        if (iVar2 == 0) {
            uVar3 = func_02009900();
            uVar1 = func_02034f64(uVar1);
            func_0202f7e8(uVar3,uVar1,5,0);
            uVar1 = func_02008444();
            func_02011080(uVar1,3);
            uVar1 = func_02011264();
            uVar1 = func_0201123c(uVar1,0);
            func_02022f2c(uVar1,((unsigned int)&data_020cd9d8),1);
            uVar1 = func_02008444();
            func_02032644(uVar1,0x1e);
            return;
        }
    }
    else {
        iVar2 = func_020790c8(uVar1,((unsigned int)&data_0209a424));
        if (iVar2 == 0) {
            uVar1 = func_02009900();
            func_0202fad4(uVar1,((unsigned int)&data_020cd9f4));
            uVar1 = func_02011264();
            uVar1 = func_0201123c(uVar1,0);
            func_02022f2c(uVar1,((unsigned int)&data_020cd7b4),0);
        }
    }
    return;
}
