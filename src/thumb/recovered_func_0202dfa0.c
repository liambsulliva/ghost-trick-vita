extern int func_02042718();

unsigned char * func_0202dfa0(unsigned char *param_1)

{
    *param_1 = 0;
    param_1[1] = 0;
    *(unsigned int *)(param_1 + 4) = 0;
    func_02042718(param_1 + 0x18);
    return param_1;
}
