extern int func_0200b208();
extern int func_02023f24();
extern int func_020293f8();

unsigned int * func_02056c24(unsigned int *param_1)

{
    unsigned int uVar1;

    *param_1 = ((unsigned int)0x020adc04u);
    uVar1 = func_0200b208();
    func_02023f24(uVar1,param_1[0x13]);
    func_020293f8(param_1);
    return param_1;
}
