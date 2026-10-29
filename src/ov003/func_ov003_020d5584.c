extern int func_0201123c();
extern int func_02011264();
extern int func_0201572c();
extern int func_02022f34();
extern int func_02022f4c();
extern int func_ov003_020d4818();

void func_ov003_020d5584(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_0201572c();
    if (iVar1 == 0) {
        uVar2 = func_02011264();
        uVar2 = func_0201123c(uVar2,0);
        func_02022f34(uVar2,((unsigned int)0x020d5b08u),*(unsigned int *)(param_1 + 0x184));
        uVar2 = func_02011264();
        func_0201123c(uVar2,1);
        func_02022f4c();
        *(unsigned char *)(param_1 + 0x16b) = 1;
    }
    func_ov003_020d4818(param_1);
    return;
}
