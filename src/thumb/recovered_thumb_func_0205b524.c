extern char data_020aeac4;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_0202f5e4();
extern int func_0202f644();
extern int func_0202f64c();
extern int func_02042774();
extern int func_02042b20();

unsigned int * func_0205b524(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;

    *param_1 = ((unsigned int)&data_020aeac4);
    func_0202f64c(param_1 + 10);
    func_0202f64c(param_1 + 0xb);
    func_0202f64c(param_1 + 0xc);
    if (param_1[0x23] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,param_1[0x23]);
            param_1[0x23] = 0;
        }
    }
    func_02042b20(param_1 + 0xe);
    func_02042774(param_1 + 0xe);
    func_0202f5e4(param_1);
    func_0202f644(param_1);
    return param_1;
}
