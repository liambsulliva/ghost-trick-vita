extern int func_02001c64(unsigned char *param_1);
extern int func_02006f5c(unsigned char *param_1, int param_2);

int func_02006efc(unsigned char *param_1)
{
    if (func_02001c64(param_1) != 0) {
        return func_02006f5c(param_1, 1);
    }
    return -3;
}
