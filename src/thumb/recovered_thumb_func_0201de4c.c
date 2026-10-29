typedef int code();
extern int func_02007b70();
extern int func_02007bd8();

unsigned int * func_0201de4c(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;

    *param_1 = ((unsigned int)0x020a4520u);
    if (param_1[1] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,param_1[1]);
            param_1[1] = 0;
        }
    }
    return param_1;
}
