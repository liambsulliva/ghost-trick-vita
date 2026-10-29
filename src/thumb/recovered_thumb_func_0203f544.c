extern char data_020a76ec;
typedef int code();
extern int func_02007b70();
extern int func_02007bd8();
extern int func_02008860();
extern int func_02033f44();
extern int func_0203f5b4();
extern int func_0203f91c();
extern int func_02072fa8();

unsigned int * func_0203f544(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;

    *param_1 = ((unsigned int)&data_020a76ec);
    if (param_1[0x30] != 0) {
        func_02072fa8(param_1[0x32],0xc0,0x300);
        func_0203f91c(param_1);
        func_02008860();
        func_0203f5b4();
    }
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
    func_02033f44(param_1);
    return param_1;
}
