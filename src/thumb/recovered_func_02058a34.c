typedef int code();
extern int func_02008860();
extern int func_0200c820();

void func_02058a34(unsigned int param_1)

{
    int *piVar1;

    func_02008860();
    piVar1 = (int *)func_0200c820();
    if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 0x10))(piVar1,param_1);
    }
    return;
}
