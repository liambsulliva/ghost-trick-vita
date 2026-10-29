extern int func_020373f8();
extern int func_02038048();
extern int func_020383f4();
extern int func_02038808();

unsigned int func_02038004(int param_1,int param_2)

{
    int iVar1;

    iVar1 = func_02038048(param_1);
    if ((iVar1 == 0) && ((*(char *)(param_1 + 4) == '\a' || (*(char *)(param_1 + 4) == '\x0f')))) {
        if ((param_2 == 0) || (iVar1 = func_020383f4(*(int *)(param_1 + 0x38),param_2), iVar1 != 0)) {
            func_020373f8(param_1,9,0,8);
            return 1;
        }
        func_02038808(*(int *)(param_1 + 0x38),param_2);
    }
    return 0;
}
