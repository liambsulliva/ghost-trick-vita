unsigned int func_02078448(unsigned int param_1,unsigned int param_2,unsigned int param_3);
void func_02078068(unsigned int param_1,unsigned int *param_2);
void func_02077fe4(void);

unsigned int func_020784ac(unsigned int param_1)
{
    unsigned int local;
    unsigned int r;

    r = func_02078448(param_1,(unsigned int)func_02078068,(unsigned int)&local);
    if (r != 0) {
        return r;
    }
    func_02077fe4();
    return local;
}
