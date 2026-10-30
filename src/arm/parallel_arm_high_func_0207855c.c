unsigned int func_020784d4(int param_1,int param_2,unsigned int param_3,unsigned int param_4);
void func_02078068(unsigned int param_1,unsigned int *param_2);
void func_02077fe4(void);

unsigned int func_0207855c(int param_1,int param_2)
{
    unsigned int local;
    unsigned int r;

    r = func_020784d4(param_1,param_2,(unsigned int)func_02078068,(unsigned int)&local);
    if (r != 0) {
        return r;
    }
    func_02077fe4();
    return local;
}
