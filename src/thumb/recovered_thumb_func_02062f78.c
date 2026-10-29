extern char data_020af8e4;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_0202f644();
extern int func_02056c6c();

unsigned int * func_02062f78(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;

    *param_1 = ((unsigned int)&data_020af8e4);
    if (param_1[0x26] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,param_1[0x26]);
            param_1[0x26] = 0;
        }
    }
    func_02056c6c(param_1);
    func_0202f644(param_1);
    return param_1;
}
