extern unsigned int func_0206d9ec(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int *param_4, unsigned int param_5);
extern unsigned int func_0206d86c(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);

unsigned int func_0206dd88(unsigned int param_1, unsigned int param_2, unsigned short param_3, unsigned int param_4)
{
    unsigned int local;
    unsigned int r;

    local = 0;
    r = func_0206d9ec(param_1, param_3, param_4, &local, 1);
    if (r != 0) {
        return r;
    }
    return func_0206d86c(param_1, param_2, local, 0);
}
