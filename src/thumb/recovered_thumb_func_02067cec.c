extern int func_02010b7c();
extern int func_0202f17c();
extern int func_02033f44();
extern int func_02067ce0();

unsigned int * func_02067cec(unsigned int *param_1)

{
    unsigned int uVar1;

    *param_1 = ((unsigned int)0x020b15ccu);
    uVar1 = func_02010b7c();
    func_0202f17c(uVar1,param_1 + 0x2f);
    func_02067ce0(param_1 + 0x2f);
    func_02033f44(param_1);
    return param_1;
}
