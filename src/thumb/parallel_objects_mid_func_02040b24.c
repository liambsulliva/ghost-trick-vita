extern int func_02040570();
extern unsigned int *func_02008444();
extern int func_0200f330();
extern int func_0202ff58();
extern int func_02010dc4();
extern int func_02040b68();
extern int func_02008418();

void func_02040b24(int param_1)
{
    unsigned int *puVar1;
    int iVar2;
    unsigned int uVar3;

    func_02040570(param_1);
    puVar1 = func_02008444();
    iVar2 = func_0200f330(puVar1);
    if (iVar2 != 0) {
        uVar3 = func_0202ff58(*(int *)(param_1 + 0x50));
        if (uVar3 != 0) {
            func_02040b68(*(int *)(param_1 + 0x50), 0x12d2);
            func_02008418(*(int *)(param_1 + 0x50));
            return;
        }
    }
    else {
        uVar3 = func_0202ff58(*(int *)(param_1 + 0x50));
        if (uVar3 == 0) {
            func_02010dc4(*(int *)(param_1 + 0x50));
        }
    }
    return;
}
