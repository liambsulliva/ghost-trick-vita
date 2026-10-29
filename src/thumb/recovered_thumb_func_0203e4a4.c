extern char data_020cec54;
extern char data_020cf5a4;
extern int func_02008444();
extern int func_02009900();
extern int func_02010ef4();
extern int func_0202f7e8();
extern int func_0203ba84();
extern int func_0204e2a0();
extern int func_02055b60();

void func_0203e4a4(int param_1)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02055b60(((unsigned int)&data_020cf5a4));
    *(unsigned int *)(param_1 + 0x24) = uVar1;
    uVar1 = func_02009900();
    func_0202f7e8(uVar1,*(unsigned int *)(param_1 + 0x24),7,0);
    iVar2 = func_0204e2a0(((unsigned int)&data_020cec54));
    *(int *)(param_1 + 0x28) = iVar2;
    if (iVar2 != 0) {
        func_0203ba84(iVar2,((unsigned int)0xfff80000u),0,0x1e);
        uVar1 = func_02009900();
        func_0202f7e8(uVar1,*(unsigned int *)(param_1 + 0x28),7,0);
    }
    uVar1 = func_02008444();
    func_02010ef4(uVar1,0xff,0xffffffff);
    *(unsigned char *)(param_1 + 4) = 5;
    return;
}
