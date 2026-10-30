extern int func_02058b4c(int param_1);

int func_020616dc(int param_1, int param_2)
{
    if (param_2 != 0 && func_02058b4c(param_2) != 0) {
        return 1;
    }
    return 0;
}
