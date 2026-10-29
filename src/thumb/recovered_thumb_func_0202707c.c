extern int func_02008828();
extern int func_02027c28();
extern int func_02051620();

void func_0202707c(unsigned int param_1,int param_2,int param_3)

{
    int iVar1;
    unsigned int uVar2;
    unsigned int uVar3;

    if ((param_2 != 0) && (iVar1 = func_02008828(param_2,((unsigned int)0x020cee88u)), iVar1 != 0)) {
        uVar2 = func_02027c28(*(unsigned short *)(param_3 + 4));
        uVar3 = func_02027c28(*(unsigned short *)(param_3 + 6));
        func_02051620(param_2,uVar2,uVar3);
    }
    return;
}
