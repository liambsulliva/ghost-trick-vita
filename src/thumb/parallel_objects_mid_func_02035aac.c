extern void * func_02011788();
extern int func_02029640();

unsigned int * func_02035aac(unsigned int *param_1)

{
    int iVar1;

    *param_1 = ((unsigned int)0x020a5d94u);
    iVar1 = func_02029640(param_1);
    if (iVar1 != 0) {
        *(unsigned int *)(iVar1 + 0x2c) = 0;
    }
    func_02011788(param_1);
    return param_1;
}
