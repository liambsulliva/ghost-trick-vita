extern char data_020a428c;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_02016e24();
extern int func_02017868();
extern int func_020187e8();

unsigned int * func_020179a8(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;

    *param_1 = ((unsigned int)&data_020a428c);
    if (param_1[0x1a] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,param_1[0x1a]);
            param_1[0x1a] = 0;
        }
    }
    if (param_1[0x1b] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,param_1[0x1b]);
            param_1[0x1b] = 0;
        }
    }
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
    func_020187e8(param_1);
    func_02017868(param_1 + 0x2b);
    func_02016e24(param_1);
    return param_1;
}
