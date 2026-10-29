extern int func_02009900();
extern int func_0200ee54();
extern int func_0202fad4();

void func_0200f918(int param_1,int param_2)

{
    int iVar1;
    unsigned int uVar2;

    uVar2 = func_02009900();
    func_0202fad4(uVar2,((unsigned int)0x020cf93cu));
    func_0200ee54(param_1,0);
    iVar1 = ((int)0x00000da4u);
    if (param_2 != 0) {
        *(unsigned int *)(param_1 + ((int)0x00000da4u) + -0x10) = *(unsigned int *)(param_1 + ((int)0x00000da4u));
        *(unsigned int *)(param_1 + iVar1 + -0xc) = *(unsigned int *)(param_1 + iVar1 + 4);
        *(unsigned int *)(param_1 + 8) = *(unsigned int *)(param_1 + 8) | 0x80;
    }
    return;
}
