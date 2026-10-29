void func_020267ca(unsigned char *param_1,unsigned int param_2,unsigned char param_3)

{
    param_1[1] = param_3;
    *param_1 = (signed char)param_2;
    param_1[2] = (signed char)((unsigned int)param_2 >> 0x10);
    param_1[3] = (signed char)((unsigned int)param_2 >> 0x18);
    return;
}
