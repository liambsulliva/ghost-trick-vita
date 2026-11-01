extern unsigned int data_020cff8c[];
extern unsigned int data_020b1714[];
extern unsigned int data_020b1718[];
extern void func_02068ee8(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);
extern void func_02068df8(void);
extern void func_02068e7c(void);

void func_02068da0(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4, int param_5)
{
    data_020cff8c[3] = param_1;
    data_020cff8c[4] = param_2;
    data_020cff8c[5] = param_3;
    data_020cff8c[6] = param_4;
    func_02068ee8(param_1, param_2, param_3, param_4);
    if (param_5 != 0) {
        data_020b1714[0] = (unsigned int)func_02068df8;
        data_020b1718[0] = (unsigned int)func_02068e7c;
    }
}
