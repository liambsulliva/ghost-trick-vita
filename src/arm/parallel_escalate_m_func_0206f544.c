struct pair
{
    unsigned int a;
    unsigned int b;
};

extern struct pair data_020b1744;
extern int func_02072cc4();
extern int func_02075c98();
extern int func_02075cac();

unsigned int func_0206f544(unsigned int param_1)
{
    unsigned int old;
    int enabled;
    unsigned int result;

    old = data_020b1744.b;
    if (old != 0xffffffff)
    {
        func_02072cc4(old);
    }
    enabled = func_02075c98();
    data_020b1744.b = param_1;
    func_02075cac(enabled);
    result = 0xffffffff;
    if (old <= 3)
    {
        result = old;
    }
    return result;
}
