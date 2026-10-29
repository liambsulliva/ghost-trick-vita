extern int func_020312a4();
extern int func_02047198();

unsigned int * func_02048378(unsigned int *param_1)

{
    *param_1 = ((unsigned int)0x020a8ebcu);
    if (param_1[0x2e] != 0) {
        func_020312a4();
        param_1[0x2e] = 0;
    }
    func_02047198(param_1);
    return param_1;
}
