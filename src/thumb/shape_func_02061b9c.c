extern unsigned char data_02080e8a[];

unsigned int func_02061b9c(unsigned int param_1,int param_2)

{
    if ((data_02080e8a[(param_2) * 8] & 2) == 0) {
        return 0;
    }
    return 1;
}
