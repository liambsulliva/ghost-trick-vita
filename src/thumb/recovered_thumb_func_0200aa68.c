typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_0202f5e4();
extern int func_0202f644();

unsigned int * func_0200aa68(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;

    *param_1 = ((unsigned int)0x020a2c20u);
    if (param_1[10] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,param_1[10]);
            param_1[10] = 0;
        }
    }
    func_0202f5e4(param_1);
    func_0202f644(param_1);
    return param_1;
}
