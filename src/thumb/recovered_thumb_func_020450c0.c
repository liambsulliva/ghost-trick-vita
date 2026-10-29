extern int func_02027424();
extern int func_02044750();

void func_020450c0(int param_1)

{
    int iVar1;

    iVar1 = func_02027424(*(unsigned int *)(param_1 + 0x120));
    if (iVar1 != 0) {
        *(unsigned char *)(param_1 + 0x11b) = 1;
    }
    *(unsigned char *)(param_1 + ((int)0x0000010au)) = 0;
    func_02044750(param_1);
    return;
}
