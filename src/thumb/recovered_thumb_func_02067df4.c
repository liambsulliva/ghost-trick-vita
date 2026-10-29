extern int func_02038ac4();
extern int func_02038acc();
extern int func_02038ae8();

unsigned int *
func_02067df4(unsigned int *param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4,
                          unsigned int param_5,unsigned int param_6)

{
    func_02038ae8();
    *param_1 = ((unsigned int)0x020b15b4u);
    func_02038ac4(param_1);
    func_02038acc(param_1,2);
    param_1[7] = param_2;
    param_1[5] = param_5;
    param_1[3] = param_3;
    param_1[6] = param_6;
    param_1[4] = param_4;
    return param_1;
}
