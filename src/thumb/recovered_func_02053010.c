extern int func_0200bb8c();
extern int func_020280a8();
extern int func_02053038();

void func_02053010(int param_1)

{
    int iVar1;

    func_0200bb8c(param_1 + 0x28,0x86);
    iVar1 = func_02053038(param_1 + 0x28);
    if (iVar1 != 4) {
        *(char *)(param_1 + 0x13) = (char)iVar1;
        func_020280a8(param_1 + 0x28,4);
    }
    return;
}
