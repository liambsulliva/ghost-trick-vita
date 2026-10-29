extern char data_020a9504;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_02008444();
extern int func_02031b04();
extern int func_02047198();

unsigned int * func_02049d08(unsigned int *param_1)

{
    unsigned int uVar1;
    int iVar2;
    int *piVar3;

    *param_1 = ((unsigned int)&data_020a9504);
    uVar1 = func_02008444();
    func_02031b04(uVar1,param_1 + 0x2f);
    if (param_1[0x42] != 0) {
        func_02007bd8();
        iVar2 = func_02007b70();
        if (iVar2 != 0) {
            func_02007bd8();
            piVar3 = (int *)func_02007b70();
            (**(code **)(*piVar3 + 0x10))(piVar3,param_1[0x42]);
            param_1[0x42] = 0;
        }
    }
    func_02047198(param_1);
    return param_1;
}
