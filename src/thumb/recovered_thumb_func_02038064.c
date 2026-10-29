extern int func_020373f8();
extern int func_02038048();

unsigned int func_02038064(int param_1)

{
    int iVar1;

    iVar1 = func_02038048();
    if (iVar1 == 0) {
        if (*(char *)(param_1 + 4) == '\a') {
            func_020373f8(param_1,0xf,0,4);
        }
        else if (*(char *)(param_1 + 4) != '\x0f') {
            return 0;
        }
    }
    return 1;
}
