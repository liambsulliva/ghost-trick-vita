void func_02079268(int *param_1,unsigned char param_2)

{
    if (*param_1 != 0) {
        *(unsigned char *)param_1[1] = param_2;
        *param_1 = *param_1 + -1;
    }
    param_1[1] = param_1[1] + 1;
    return;
}
