extern int func_02007aa8();
extern int func_0200b154();
extern int func_0200b4e0();

unsigned int * func_0200b460(unsigned int *param_1)

{
    func_02007aa8();
    *param_1 = ((unsigned int)0x020a2cb8u);
    param_1[3] = 0x80000000;
    param_1[4] = 0x80000000;
    func_0200b154(param_1 + 5);
    param_1[0x21] = 0;
    func_0200b4e0(param_1);
    return param_1;
}
