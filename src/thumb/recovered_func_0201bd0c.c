extern int func_020312a4();

int func_0201bd0c(int param_1)

{
    if (*(int *)(param_1 + 8) != 0) {
        func_020312a4();
        *(unsigned int *)(param_1 + 8) = 0;
    }
    return param_1;
}
