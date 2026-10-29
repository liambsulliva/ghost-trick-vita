extern int func_02008824();
extern int func_0200883c();
extern int func_02018710();
extern int func_0201c0f8();
extern int func_0201e7f8();

void func_0201c0bc(unsigned int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;
    int iVar2;
    unsigned int uVar3;
    unsigned short auStack_30 [16];

    func_0200883c();
    uVar1 = func_02008824();
    uVar3 = 0;
    func_0201e7f8(uVar1,0,param_3,auStack_30);
    func_02018710(param_2);
    iVar2 = func_0201c0f8();
    do {
        *(unsigned short *)(iVar2 + uVar3 * 2) = auStack_30[uVar3];
        uVar3 = uVar3 + 1 & 0xff;
    } while (uVar3 < 0x10);
    return;
}
