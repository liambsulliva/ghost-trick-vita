extern char data_020af480;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_02042774();
extern int func_02056cac();

unsigned int * func_0205c2c0(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;

    *param_1 = ((unsigned int)&data_020af480);
    if (param_1[0x32] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,param_1[0x32]);
            param_1[0x32] = 0;
        }
    }
    if (param_1[0x33] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,param_1[0x33]);
            param_1[0x33] = 0;
        }
    }
    func_02042774(param_1 + 0x34);
    func_02056cac(param_1);
    return param_1;
}
