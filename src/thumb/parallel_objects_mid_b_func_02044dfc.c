void func_02044dfc(unsigned char *param_1, unsigned char param_2, unsigned char param_3)
{
    int t;
    unsigned char *q;
    unsigned char *w;
    t = param_1[0xe0];
    t = t & ~1;
    t = t | 1;
    param_1[0xe0] = t;
    q = param_1 + 0xec;
    w = param_1 + 0xed;
    *q = param_2;
    *w = param_3;
}
