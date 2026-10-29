extern int func_02018740();
extern int func_02018784();
extern int func_02018d3c();

unsigned int func_02018714(int param_1)

{
    int iVar1;

    iVar1 = func_02018d3c();
    if (iVar1 == 1) {
        return 1;
    }
    if (*(char *)(param_1 + 0x8c) == '\0') {
        func_02018740(param_1);
    }
    else {
        func_02018784(param_1);
    }
    return 1;
}
