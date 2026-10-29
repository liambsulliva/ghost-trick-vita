extern char data_0209b244;
extern char data_0209b278;
extern int func_02008444();
extern int func_02008824();
extern int func_0200883c();
extern int func_02009c1c();
extern int func_0200cb5c();
extern int func_0201e888();
extern int func_0202f1bc();
extern int func_020322d4();

void func_020409ac(void)

{
    int iVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    unsigned int uVar4;
    unsigned int *puVar5;

    puVar5 = ((unsigned int *)&data_0209b244);
    func_02008444();
    iVar1 = func_020322d4();
    if (iVar1 != 0) {
        puVar5 = ((unsigned int *)&data_0209b278);
    }
    for (; uVar4 = *puVar5, uVar4 != 0; puVar5 = puVar5 + 4) {
        func_0200883c();
        uVar2 = func_02008824();
        iVar1 = func_0201e888(uVar2,puVar5[1] & 0xffff,uVar4 & 0xffff,1,0);
        if (iVar1 != 0) {
            uVar2 = func_02009c1c();
            func_0200883c();
            func_02008824();
            uVar3 = func_0200cb5c();
            func_0202f1bc(uVar2,iVar1,puVar5[2],uVar3,puVar5[3]);
        }
    }
    return;
}
