extern char data_020cd9d8;
extern char data_020cfe9c;
extern int func_02008444();
extern int func_02009900();
extern int func_020108ec();
extern int func_02010ef4();
extern int func_02010fbc();
extern int func_02011080();
extern int func_02011320();
extern int func_02022fc0();
extern int func_0202fad4();
extern int func_02031b20();
extern int func_0203571c();

void func_0203389c(int param_1)

{
    unsigned char uVar1;
    unsigned int uVar2;
    int iVar3;

    func_02008444();
    uVar1 = func_02011320();
    *(unsigned char *)(param_1 + 0x21) = uVar1;
    uVar2 = func_02008444();
    func_02011080(uVar2,6);
    uVar2 = func_02008444();
    uVar1 = func_02010fbc(uVar2,1);
    *(unsigned char *)(param_1 + 0x20) = uVar1;
    uVar2 = func_02008444();
    func_02010ef4(uVar2,0xff,0xffffffff);
    func_02008444();
    uVar1 = func_020108ec();
    *(unsigned char *)(param_1 + 4) = uVar1;
    uVar2 = func_02009900();
    func_0202fad4(uVar2,((unsigned int)&data_020cfe9c));
    uVar2 = func_02031b20(param_1);
    iVar3 = func_02022fc0(uVar2,((unsigned int)&data_020cd9d8));
    if (iVar3 != 0) {
        func_0203571c();
    }
    return;
}
