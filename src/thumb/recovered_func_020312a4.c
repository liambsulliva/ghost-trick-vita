typedef int code();
extern int func_02007b70();
extern int func_02007bd8();

void func_020312a4(int param_1)

{
    int *piVar1;

    if (param_1 != 0) {
        func_02007bd8();
        piVar1 = (int *)func_02007b70();
        (**(code **)(*piVar1 + 0x10))(piVar1,param_1);
    }
    return;
}
