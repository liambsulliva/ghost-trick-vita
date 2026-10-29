extern int func_02020cf4();

unsigned int func_02020ce0(unsigned int *param_1)

{
    unsigned int uVar1;
    unsigned int uVar2;

    uVar2 = *param_1;
    uVar1 = func_02020cf4(uVar2);
    *param_1 = uVar1;
    return uVar2;
}
