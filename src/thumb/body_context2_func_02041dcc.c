struct State {
    unsigned int before_flags[21];
    unsigned int flags;
};
extern struct State data_020c9e48;
extern char data_020c9e88;
extern int func_02008824();
extern int func_0200883c();
extern int func_02009c1c();
extern int func_0201e888();
extern int func_0202f1bc();

void func_02041dcc(void)

{
    unsigned int uVar1;
    unsigned int uVar2;
    int iVar3;
    int iVar4;
    unsigned int uVar5;
    int iVar6;
    unsigned int uVar7;

    data_020c9e48.flags = ((unsigned int)0x3ffff0f8u) & data_020c9e48.flags | 0x200;
    *(unsigned short *)(((int)&data_020c9e88) + 0x1a) = 0x201;
    func_0200883c();
    uVar1 = func_02008824();
    uVar7 = 0;
    uVar1 = func_0201e888(uVar1,3,0x1b00,1,0);
    uVar2 = func_02009c1c();
    func_0202f1bc(uVar2,uVar1,0x2000,0x6000,0x17);
    func_0200883c();
    uVar1 = func_02008824();
    uVar1 = func_0201e888(uVar1,0,0x3c,1,0);
    uVar2 = func_02009c1c();
    func_0202f1bc(uVar2,uVar1,0,0x20,0x15);
    func_0200883c();
    uVar1 = func_02008824();
    iVar3 = func_0201e888(uVar1,2,0xb,1,0);
    if (iVar3 != 0) {
        do {
            uVar5 = 0;
            iVar6 = iVar3 + uVar7 * 0x40;
            do {
                iVar4 = uVar5 * 2;
                uVar5 = uVar5 + 1;
                *(unsigned short *)(iVar6 + iVar4) = *(unsigned short *)(iVar6 + iVar4) + 0x100;
            } while (uVar5 < 0x20);
            uVar7 = uVar7 + 1;
        } while (uVar7 < 0x18);
        uVar1 = func_02009c1c();
        func_0202f1bc(uVar1,iVar3,0,0x800,0x1b);
    }
    return;
}
