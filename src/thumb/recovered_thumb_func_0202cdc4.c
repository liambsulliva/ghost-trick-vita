extern char data_020a5198;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_0202b400();
extern int func_0202b56c();

unsigned int * func_0202cdc4(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;

    *param_1 = ((unsigned int)&data_020a5198);
    if (param_1[0x38] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,param_1[0x38]);
            param_1[0x38] = 0;
        }
    }
    func_0202b56c(param_1);
    func_0202b400(param_1);
    return param_1;
}
