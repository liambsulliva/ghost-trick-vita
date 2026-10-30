extern char data_040004a8;

void func_0206322c(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6,
                   int param_7,unsigned int param_8)
{
    *((unsigned int *)&data_040004a8) =
        param_7 << 0x1d |
        (param_2 << 0x1e | (param_8 >> 3 | param_1 << 0x1a) | param_3 << 0x14 | param_4 << 0x17 |
         param_5 << 0x10 | param_6 << 0x12);
    return;
}
