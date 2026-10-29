extern int func_02009900();
extern int func_0202f7e8();
extern int func_0204edc8();

void func_02010dd0(int param_1)

{
    int iVar1;
    int iVar2;
    unsigned int uVar3;

    iVar1 = ((int)0x00000fb8u);
    if (*(int *)(param_1 + ((int)0x00000fb8u)) == 0) {
        iVar2 = func_0204edc8(((unsigned int)0x020ced20u));
        *(int *)(param_1 + iVar1) = iVar2;
        if (iVar2 != 0) {
            uVar3 = func_02009900();
            func_0202f7e8(uVar3,iVar2,1,0);
        }
    }
    return;
}
