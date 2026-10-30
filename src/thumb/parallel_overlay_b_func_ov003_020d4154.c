extern unsigned int func_ov003_020d414c(unsigned int);

unsigned int func_ov003_020d4154(unsigned int param_1)
{
    unsigned int count = func_ov003_020d414c(param_1);
    return param_1 & ((int)0x80000000 >> count);
}
