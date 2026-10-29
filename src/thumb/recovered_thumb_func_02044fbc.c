extern char data_020a88e4;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_02008444();
extern int func_02031b04();
extern int func_020446a0();

unsigned int * func_02044fbc(unsigned int *param_1)

{
    unsigned int uVar1;
    int iVar2;
    int *piVar3;

    *param_1 = ((unsigned int)&data_020a88e4);
    uVar1 = func_02008444();
    func_02031b04(uVar1,param_1 + 0x4b);
    if (param_1[0x45] != 0) {
        func_02007bd8();
        iVar2 = func_02007b70();
        if (iVar2 != 0) {
            func_02007bd8();
            piVar3 = (int *)func_02007b70();
            (**(code **)(*piVar3 + 0x10))(piVar3,param_1[0x45]);
            param_1[0x45] = 0;
        }
    }
    func_020446a0(param_1);
    return param_1;
}
