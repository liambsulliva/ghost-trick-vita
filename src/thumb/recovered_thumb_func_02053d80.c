extern int func_02008444();
extern int func_0202f644();
extern int func_02031b04();
extern int func_02053bfc();

unsigned int * func_02053d80(unsigned int *param_1)

{
    unsigned int uVar1;

    *param_1 = ((unsigned int)0x020ad150u);
    uVar1 = func_02008444();
    func_02031b04(uVar1,param_1 + 0x56);
    func_02053bfc(param_1);
    func_0202f644(param_1);
    return param_1;
}
