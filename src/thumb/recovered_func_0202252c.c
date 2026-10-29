extern int func_020312a4();

int func_0202252c(int param_1)

{
    if (*(int *)(param_1 + 0xc) != 0) {
        func_020312a4();
        *(unsigned int *)(param_1 + 0xc) = 0;
    }
    return param_1;
}
