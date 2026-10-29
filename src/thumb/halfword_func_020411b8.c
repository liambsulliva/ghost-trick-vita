extern int func_020411e4();

void func_020411b8(unsigned short *param_1,unsigned short param_2)

{
    param_1[1] = param_2;
    if (*param_1 == 2) {
        func_020411e4(param_1);
        *param_1 = 1;
    }
    return;
}
