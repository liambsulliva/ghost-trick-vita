extern int func_02008444();
extern int func_020151b0();
extern int func_02054a6c();
extern int func_02054aec();

void func_020549f4(int param_1)

{
    int iVar1;

    func_02008444();
    iVar1 = func_020151b0();
    if (iVar1 == 0) {
        *(unsigned int *)(param_1 + 0x38) = 0;
        *(unsigned char *)(param_1 + 0x10) = 2;
        func_02054aec(param_1);
        func_02054a6c(param_1);
    }
    return;
}
