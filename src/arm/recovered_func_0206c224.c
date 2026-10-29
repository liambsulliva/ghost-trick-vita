extern int func_0206b2ac();
extern int func_0206b728();
extern int func_0206b8d4();
extern int func_02074120();
extern int func_02075c98();
extern int func_02075cac();

void func_0206c224(int param_1,unsigned int param_2)

{
    unsigned int uVar1;
    int iVar2;

    iVar2 = *(int *)(param_1 + 8);
    if ((*(unsigned int *)(iVar2 + 0xc) & 4) != 0) {
        uVar1 = func_02075c98();
        *(unsigned int *)(iVar2 + 0xc) = *(unsigned int *)(iVar2 + 0xc) | 8;
        *(unsigned int *)(iVar2 + 0x14) = param_2;
        func_02074120(iVar2 + 0x18);
        func_02075cac(uVar1);
        return;
    }
    func_0206b2ac(iVar2);
    iVar2 = func_0206b728(param_1,1);
    if (iVar2 == 0) {
        return;
    }
    func_0206b8d4();
    return;
}
