extern int func_0201572c();
extern int func_ov003_020d4818();

void func_ov003_020d50ec(int param_1)

{
    int iVar1;

    func_ov003_020d4818();
    iVar1 = func_0201572c();
    if (iVar1 == 0) {
        *(unsigned char *)(param_1 + 0x168) = 1;
    }
    return;
}
