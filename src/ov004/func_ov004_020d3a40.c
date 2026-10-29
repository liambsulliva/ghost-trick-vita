void func_ov004_020d3a40(int *param_1,unsigned char *param_2)

{
    param_1[1] = (unsigned int)*param_2;
    *param_1 = (int)*(short *)(param_2 + 2);
    return;
}
