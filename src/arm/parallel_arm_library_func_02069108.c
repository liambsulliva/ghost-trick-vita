extern unsigned int data_020cffa8[];
extern unsigned int data_020b171c[];
extern unsigned int data_020b1720[];
extern void func_02069274(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);
extern void func_0206915c(void);
extern void func_02069230(void);

void func_02069108(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4)
{
    data_020cffa8[2] = param_1;
    data_020cffa8[3] = param_2;
    data_020cffa8[4] = param_3;
    func_02069274(param_1, param_2, param_3, param_4);
    if (param_4 != 0) {
        data_020b171c[0] = (unsigned int)func_0206915c;
        data_020b1720[0] = (unsigned int)func_02069230;
    }
}
