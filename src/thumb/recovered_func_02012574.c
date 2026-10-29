extern int func_02007aa8();
extern int func_02012598();

unsigned int * func_02012574(unsigned int *param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;

    func_02007aa8();
    uVar1 = ((unsigned int)0x020a2eccu);
    param_1[1] = param_2;
    *param_1 = uVar1;
    param_1[2] = param_3;
    func_02012598(param_1);
    return param_1;
}
