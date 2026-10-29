extern char data_020cee88;
extern int func_02009900();
extern int func_02027c28();
extern int func_0202f7e8();
extern int func_020507b0();
extern int func_02050c28();
extern int func_02051acc();
extern int func_02051ad0();
extern int func_02051ad4();

void func_02051a70(unsigned int param_1,unsigned int param_2)

{
    int iVar1;
    int iVar2;
    int iVar3;
    unsigned int uVar4;

    iVar1 = func_020507b0(((unsigned int)&data_020cee88));
    if (iVar1 != 0) {
        iVar2 = func_02051acc(param_1);
        iVar3 = func_02051ad0(param_1);
        uVar4 = func_02027c28(*(unsigned short *)(iVar2 + iVar3 * 2 + 8));
        *(unsigned int *)(iVar1 + 0xfc) = uVar4;
        *(unsigned int *)(iVar1 + 0xb0) = param_2;
        func_02050c28(iVar1,iVar2,iVar3 + 5);
        uVar4 = func_02009900();
        func_0202f7e8(uVar4,iVar1,5,0);
    }
    func_02051ad4(param_1);
    return;
}
