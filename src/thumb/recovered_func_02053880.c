extern int func_02008444();
extern int func_02031b04();
extern int func_02052d00();

unsigned int * func_02053880(unsigned int *param_1)

{
    unsigned int uVar1;

    *param_1 = ((unsigned int)0x020acdccu);
    uVar1 = func_02008444();
    func_02031b04(uVar1,param_1 + 0x56);
    func_02052d00(param_1);
    return param_1;
}
