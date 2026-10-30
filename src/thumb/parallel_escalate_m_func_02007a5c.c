extern void func_02076680(unsigned int param_1,unsigned int param_2,unsigned int param_3);
void func_02007a5c(unsigned int param_1,int param_2,unsigned int param_3)
{
    if (param_2 < 0) {
        param_2 = 0;
    }
    else {
        param_2 = param_2 & 0x7f;
    }
    func_02076680(param_1,param_2,param_3);
}
