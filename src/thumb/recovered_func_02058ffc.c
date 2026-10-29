extern int func_0200b598();
extern int func_0200ba28();
extern int func_02010b54();

void func_02058ffc(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_02010b54();
    if (iVar1 != 0) {
        uVar2 = func_0200ba28(param_1 + 0x28);
        func_0200b598(param_1 + 0x28,uVar2,1);
    }
    return;
}
