extern int func_0200b598();
extern int func_02010b54();
extern int func_02029ab4();
extern int func_0205953c();

void func_02059a0c(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    func_0205953c(param_1,*(unsigned char *)(param_1 + 0xc3));
    iVar1 = func_02010b54(param_1);
    if (iVar1 != 0) {
        uVar2 = func_02029ab4(param_1 + 0x114);
        func_0200b598(param_1 + 0x28,uVar2,1);
    }
    return;
}
