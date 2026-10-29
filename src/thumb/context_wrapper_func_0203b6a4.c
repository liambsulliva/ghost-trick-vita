extern char data_020cf438;
extern char data_020cf75c;
extern int func_02008444();
extern int func_02008828();
extern int func_02009900();
extern int func_0200e62c();
extern int func_0200eab4();
extern int func_0200eab8();
extern int func_0200eac4();
extern int func_02010ef4();
extern int func_0202f7e8();
extern int func_0202fbb0();
extern int func_0200eabc();
extern int func_0203b724();
extern int func_02054060();

void func_0203b6a4(int param_1)

{
    unsigned int uVar1;
    unsigned int uVar2;
    int iVar3;
    unsigned char auStack_20 [16];

    func_02008444();
    func_0200e62c();
    uVar1 = func_02054060(((unsigned int)&data_020cf438));
    *(unsigned int *)(param_1 + 0x24) = uVar1;
    uVar1 = func_02009900();
    func_0202f7e8(uVar1,*(unsigned int *)(param_1 + 0x24),7,0);
    if (*(char *)(param_1 + 0x2e) != '\0') {
        uVar1 = func_02008444();
        func_02010ef4(uVar1,0xff,0xffffffff);
    }
    func_0202fbb0(auStack_20);
    uVar1 = ((unsigned int)&data_020cf75c);
    while (iVar3 = func_0200eac4(auStack_20), iVar3 != 0) {
        uVar2 = func_0200eab4(auStack_20);
        iVar3 = func_02008828(uVar2,uVar1);
        if (iVar3 != 0) {
            func_0200eab8(auStack_20);
            func_0203b724();
        }
        func_0200eabc(auStack_20,0);
    }
    return;
}
