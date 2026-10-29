extern int func_02008444();
extern int func_02009bfc();
extern int func_0200ec24();
extern int func_0200ecb0();
extern int func_0201192c();
extern int func_02011a14();
extern int func_02011a28();
extern int func_020267ac();
extern int func_020267b8();
extern int func_0203247c();
extern int func_02071960();
extern int func_02071dd0();
extern int func_02071e40();

void func_0203dd48(int param_1)

{
    unsigned int uVar1;
    unsigned int uVar2;
    int iVar3;

    uVar1 = func_02008444();
    func_02008444();
    uVar2 = func_0203247c();
    func_0201192c(uVar1,uVar2);
    iVar3 = func_02071e40();
    if (*(int *)(param_1 + 0x40) != iVar3) {
        func_02071dd0();
        func_02071960(*(unsigned int *)(param_1 + 0x40));
    }
    uVar1 = func_02008444();
    func_0200ec24(uVar1,0);
    uVar1 = func_02008444();
    func_0200ecb0(uVar1,*(unsigned short *)(param_1 + 0x20),0x80000);
    uVar1 = func_02008444();
    func_02011a14(uVar1,*(unsigned int *)(param_1 + 0x24));
    uVar1 = func_02008444();
    func_02011a28(uVar1,*(unsigned int *)(param_1 + 0x28));
    uVar1 = func_02008444();
    func_020267ac(uVar1,*(unsigned int *)(param_1 + 0x24));
    uVar1 = func_02008444();
    func_020267b8(uVar1,*(unsigned int *)(param_1 + 0x28));
    uVar1 = func_02008444();
    func_02009bfc(uVar1,param_1 + 0x2c);
    return;
}
