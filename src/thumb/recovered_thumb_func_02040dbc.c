extern char data_020a7ba0;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_0203fb14();

unsigned int * func_02040dbc(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;

    *param_1 = ((unsigned int)&data_020a7ba0);
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
    func_0203fb14(param_1);
    return param_1;
}
