int func_0200c40c(unsigned short *param_1)

{
    return (int)((unsigned int)*param_1 * 0x1000000 + ((unsigned int)((int)((unsigned int)*param_1 * 0x1000000) >> 0x17) >> 8)
                            ) >> 0x18;
}
