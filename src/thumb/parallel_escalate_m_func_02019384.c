extern char data_020a40f8;
extern int func_02016c4c();

int *func_02019384(int *param_1, int param_2)
{
    func_02016c4c(param_1);
    *param_1 = (int)&data_020a40f8;
    param_1[0x1a] = param_2;
    param_1[0x1b] = 0;
    param_1[0x1c] = 0;
    param_1[0x1d] = 0;
    return param_1;
}
