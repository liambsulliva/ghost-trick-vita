extern char data_020afb30;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_02056c6c();
extern int func_02062c0c();

unsigned int * func_02062288(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;
    unsigned int uVar3;

    *param_1 = ((unsigned int)&data_020afb30);
    if (param_1[0x3c] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,param_1[0x3c]);
            param_1[0x3c] = 0;
        }
    }
    uVar3 = 0;
    do {
        if (param_1[uVar3 + 0x2c] != 0) {
            func_02062c0c(param_1[uVar3 + 0x2c],param_1);
        }
        uVar3 = uVar3 + 1;
    } while (uVar3 < 0x10);
    func_02056c6c(param_1);
    return param_1;
}
