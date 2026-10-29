extern int func_020088d8();
extern int func_02031270();

unsigned int * func_0201e494(unsigned int *param_1)

{
    *param_1 = ((unsigned int)0x020a456cu);
    if (*(char *)(param_1 + 6) == '\0') {
        func_02031270(param_1[3]);
    }
    func_020088d8(param_1);
    func_02031270(param_1);
    return param_1;
}
