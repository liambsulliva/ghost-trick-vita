unsigned int func_0201e0ac(unsigned int param_1,unsigned int param_2,int param_3,unsigned short param_4)

{








    if ((param_2 < 3) && (param_3 < 6)) {
        param_1 = param_1 + param_2 * 0x1c + param_3 * 4;
        *(unsigned short *)(param_1 + 0x3c) = param_4;
    }
    return param_1;
}
