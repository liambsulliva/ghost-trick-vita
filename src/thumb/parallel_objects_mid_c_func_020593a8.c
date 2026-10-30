extern unsigned int * func_02008444();
extern int func_0200bc10();
extern int func_0200f034();
extern int func_0200f05c();
extern int func_020593dc();

void func_020593a8(int param_1)

{
    int iVar1;
    unsigned int *puVar2;
    int iVar2;
    iVar1 = func_0200bc10(param_1 + 0x28);
    if (iVar1 != 0) {
        puVar2 = func_02008444();
        iVar2 = func_0200f05c((int)puVar2);
    }
    else {
        puVar2 = func_02008444();
        iVar2 = func_0200f034((int)puVar2);
    }
    func_020593dc((unsigned char *)(param_1 + 0x28),1,(unsigned char)iVar2);
    return;
}
