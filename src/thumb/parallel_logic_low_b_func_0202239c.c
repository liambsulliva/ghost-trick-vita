extern char *func_02022350(int a,char *b);

int func_0202239c(int param_1,char *param_2)
{
    char *found;

    found = func_02022350(param_1,param_2);
    if (found != 0) {
        return (int)found - *(int *)(param_1 + 4);
    }
    return -1;
}
