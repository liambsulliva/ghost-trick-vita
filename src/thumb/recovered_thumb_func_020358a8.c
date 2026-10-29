extern int func_02009900();
extern int func_0202f5e4();
extern int func_0202f644();
extern int func_0202fad4();

unsigned int * func_020358a8(unsigned int *param_1)

{
    unsigned int uVar1;

    *param_1 = ((unsigned int)0x020a5d5cu);
    uVar1 = func_02009900();
    func_0202fad4(uVar1,((unsigned int)0x020cda2cu));
    func_0202f5e4(param_1);
    func_0202f644(param_1);
    return param_1;
}
