typedef int code();

extern void func_0207a54c(int buf, int *len);
extern void func_0207a550(int *param_1);

int func_0207a578(int *param_1, int *param_2)
{
    int iVar1;

    iVar1 = param_1[9] - param_1[7];
    if (iVar1 != 0) {
        param_1[10] = iVar1;
        if (((((unsigned int)param_1[1]) << 19) >> 31) == 0) {
            func_0207a54c(param_1[7], param_1 + 10);
        }
        iVar1 = (*(code *)param_1[0x10])(param_1[0], param_1[7], param_1 + 10, param_1[0x12]);
        if (param_2 != (int *)0) {
            *param_2 = param_1[10];
        }
        if (iVar1 != 0) {
            return iVar1;
        }
        param_1[6] = param_1[6] + param_1[10];
    }
    func_0207a550(param_1);
    return 0;
}
