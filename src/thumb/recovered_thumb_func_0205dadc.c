extern int func_020293f8();
extern int func_020716e8();
extern int func_020719fc();

unsigned int * func_0205dadc(unsigned int *param_1)

{
    *param_1 = ((unsigned int)0x020aedfcu);
    if (param_1[0x12] != -1) {
        func_020716e8(4);
        func_020719fc(8);
    }
    func_020293f8(param_1);
    return param_1;
}
