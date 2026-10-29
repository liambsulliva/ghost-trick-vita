extern int func_02007aa8();
extern int func_020082d8();

unsigned int * func_02058910(unsigned int *param_1)

{
    unsigned int uVar1;

    func_02007aa8();
    *param_1 = ((unsigned int)0x020ae5e4u);
    uVar1 = ((unsigned int)0x020a46c8u);
    param_1[1] = 0;
    func_020082d8(param_1 + 2,uVar1);
    param_1[5] = 0;
    return param_1;
}
