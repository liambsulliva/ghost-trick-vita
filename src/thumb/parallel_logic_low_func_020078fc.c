extern char *data_020b1908;
extern char *data_020b1904;

void func_020078fc(void)
{
    int *p;
    int zero;

    zero = 0;
    p = (int *)data_020b1908;
    p = (int *)((char *)p + 0x5154);
    data_020b1904 = (char *)p;
    *p = zero;
    p = (int *)data_020b1904;
    p[1] = zero;
    p = (int *)data_020b1904;
    p[2] = zero;
}
