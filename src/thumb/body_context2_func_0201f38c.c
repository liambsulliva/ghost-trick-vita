typedef int code();
extern void func_0201f338();

unsigned int * func_0201f38c(unsigned int *param_1)

{
    *param_1 = ((unsigned int)0x020a4758u);
    if (((*(char *)(param_1 + 4) != '\0') && (param_1[3] != 0)) && ((int *)param_1[5] != (int *)0x0))
    {
        (**(code **)(*(int *)param_1[5] + 0x10))((int *)param_1[5],param_1[3]);
        param_1[3] = 0;
    }
    func_0201f338(param_1);
    return param_1;
}
