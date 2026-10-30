extern char *func_02008444(void);
extern int func_0201132c(char *object);
extern int func_020328e4(char *object);
extern char data_020cf09c;
extern char data_020cf0a0;
extern char data_020cf0a4;

void func_02052a7c(int *param_1)
{
    if (func_0201132c(func_02008444()) != 0) {
        param_1[0x2a] = (int)&data_020cf0a0;
        return;
    }
    if (func_020328e4(func_02008444()) != 0) {
        param_1[0x2a] = (int)&data_020cf0a4;
        return;
    }
    param_1[0x2a] = (int)&data_020cf09c;
}
