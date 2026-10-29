typedef int code();
extern int func_0202f5e4();
extern int func_0202f644();

unsigned int * func_0205bcb8(unsigned int *param_1)

{
    int *piVar1;

    *param_1 = ((unsigned int)0x020aecf8u);
    piVar1 = (int *)param_1[10];
    if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 0x20))(piVar1,0);
    }
    param_1[10] = 0;
    func_0202f5e4(param_1);
    func_0202f644(param_1);
    return param_1;
}
