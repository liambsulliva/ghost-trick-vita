typedef int code();
extern int func_02007b70();
extern int func_02007bd8();

int func_0201eac0(int param_1)

{
    int *piVar1;

    func_02007bd8();
    piVar1 = (int *)func_02007b70();
    (**(code **)(*piVar1 + 0x10))(piVar1,*(unsigned int *)(param_1 + ((int)0x0000060cu)));
    return param_1;
}
