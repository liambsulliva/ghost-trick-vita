extern int func_0200eb78();
extern int func_02022c18();

void func_02042a14(int param_1)

{
    unsigned int uVar1;
    int iVar2;

    iVar2 = *(int *)(param_1 + 4);
    if ((iVar2 != -1) && (*(int *)(param_1 + 0xc) == -1)) {
        uVar1 = func_0200eb78();
        func_02022c18(uVar1,iVar2);
        *(unsigned int *)(param_1 + 4) = 0xffffffff;
        *(unsigned char *)(param_1 + 0x38) = 0;
        *(unsigned char *)(param_1 + 0x3a) = 1;
    }
    return;
}
