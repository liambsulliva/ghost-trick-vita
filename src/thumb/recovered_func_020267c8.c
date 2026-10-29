void func_020267c8(unsigned char *param_1,unsigned int param_2)

{
    param_1[1] = (char)((unsigned int)param_2 >> 8);
    *param_1 = (char)param_2;
    param_1[2] = (char)((unsigned int)param_2 >> 0x10);
    param_1[3] = (char)((unsigned int)param_2 >> 0x18);
    return;
}
