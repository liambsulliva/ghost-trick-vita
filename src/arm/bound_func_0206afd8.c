extern char data_020d08e0;
extern char data_020d090c;
extern int func_02069bb4();
extern int func_02069bc0();
extern int func_02069c30();
extern int func_0206aae8();
extern int func_02073a04();
extern int func_02073a5c();
extern int func_02075d50();

void func_0206afd8(void)

{
    unsigned short uVar1;
    int iVar2;

    *(unsigned int *)(((int)&data_020d08e0) + 0xc) = ((unsigned int)0x0206aae8u);
    iVar2 = func_02075d50();
    if (iVar2 != 1) {
        return;
    }
    iVar2 = func_02069bb4();
    if (*(int *)(iVar2 + 0x80) == 0) {
        return;
    }
    uVar1 = func_02073a04();
    func_02069bc0(uVar1);
    iVar2 = func_02069bb4();
    func_0206aae8(0,((unsigned int)&data_020d090c),*(unsigned int *)(iVar2 + 0x80),0x88);
    func_02069c30(uVar1);
    func_02073a5c(uVar1);
    return;
}
