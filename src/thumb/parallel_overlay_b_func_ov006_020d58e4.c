extern int func_ov006_020d7240();
extern int func_02008444();
extern int func_02014cb8();
extern int func_02011264();
extern int func_0201123c();
extern int func_02022f2c();

void func_ov006_020d58e4(unsigned char *param_1)
{
    unsigned char *flag;

    flag = param_1 + 0x27;
    if (*flag == 0) {
        func_ov006_020d7240(param_1);
        func_02008444();
        func_02014cb8();
        param_1[0x24] = 11;
    }
    else if (*flag == 1) {
        func_02022f2c(func_0201123c(func_02011264(), 0), 0x020cdf78u, 0);
        param_1[0x24] = 12;
    }
}
