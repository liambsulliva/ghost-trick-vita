typedef int code();
extern int func_0200eab4();
extern int func_0200eac4();
extern int func_02010b54();
extern int func_02026d10();
extern int func_0202fbf8();
extern int func_0200eabc();

void func_0202f9ac(void)

{
    unsigned int uVar1;
    int *piVar2;
    int iVar3;

    uVar1 = func_02026d10();
    func_0202fbf8(uVar1,0);
    while (iVar3 = func_0200eac4(uVar1), iVar3 != 0) {
        func_0200eab4(uVar1);
        iVar3 = func_02010b54();
        if ((iVar3 != 0) && (iVar3 = func_0200eab4(uVar1), *(char *)(iVar3 + 0xc) == '\x03')) {
            piVar2 = (int *)func_0200eab4(uVar1);
            (**(code **)(*piVar2 + 0x1c))(piVar2);
        }
        func_0200eabc(uVar1,0);
    }
    return;
}
