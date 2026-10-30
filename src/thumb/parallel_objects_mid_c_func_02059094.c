extern int data_020a46c8;
extern int func_020083b0();

void func_02059094(int *param_1,int param_2,unsigned int param_3)
{
    param_1[1] = param_2;
    param_1[5] = (param_1[5] & ~0xc) | (int)(((param_3 << 30) >> 28));
    func_020083b0(param_1 + 2,&data_020a46c8);
    param_1[5] = (unsigned short)param_1[5];
    return;
}
