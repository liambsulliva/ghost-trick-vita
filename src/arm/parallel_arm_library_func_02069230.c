extern int data_020cffa8;
extern int data_020cffac;
extern int func_02068d08();

int func_02069230(unsigned int param_1)
{
    return func_02068d08(&data_020cffa8, &data_020cffac, (param_1 << 16) >> 13, ((param_1 & 0xffff0000) >> 16) << 3) == 0;
}
