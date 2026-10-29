extern int func_02007aa8();
extern int func_02022ed4();

unsigned int * func_02022ee0(unsigned int *param_1)

{
    func_02007aa8();
    *param_1 = ((unsigned int)0x020a4950u);
    param_1[9] = 0;
    param_1[10] = 0;
    param_1[0xc] = 0;
    func_02022ed4(param_1 + 1,0x20);
    return param_1;
}
