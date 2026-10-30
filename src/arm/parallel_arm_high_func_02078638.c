int func_02078cb0(void);
unsigned int func_02078410(unsigned int param_1,unsigned int param_2,unsigned int param_3);

int func_02078638(unsigned int param_1)
{
    if (func_02078cb0() == 0) {
        return 0;
    }
    return func_02078410(16,param_1 & 0xffff,0);
}
