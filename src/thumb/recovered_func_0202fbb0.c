extern int func_02009900();
extern int func_0202fbf8();

unsigned int * func_0202fbb0(unsigned int *param_1)

{
    unsigned int uVar1;

    *(unsigned char *)(param_1 + 3) = 0;
    uVar1 = func_02009900();
    *param_1 = uVar1;
    func_0202fbf8(param_1,0);
    return param_1;
}
