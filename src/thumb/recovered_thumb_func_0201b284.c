extern char data_020a3914;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_02016e24();
extern int func_02031270();
extern int func_0207de48();

unsigned int * func_0201b284(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;

    *param_1 = ((unsigned int)&data_020a3914);
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
    if (param_1[0x1c] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            (**(code **)(*piVar2 + 0x10))(piVar2,param_1[0x1c]);
            param_1[0x1c] = 0;
        }
    }
    func_0207de48((int)param_1 + 0x8e,8,0xe,((unsigned int)0x0201b16du));
    func_02016e24(param_1);
    func_02031270(param_1);
    return param_1;
}
