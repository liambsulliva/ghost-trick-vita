extern unsigned int *func_02008444();
extern int func_02010194();
extern int func_02058b64();

int func_020333e4(int *param_1)
{
    int iVar1;

    iVar1 = func_02010194((int)func_02008444());
    if (param_1[11] != iVar1) {
        param_1[11] = iVar1;
        if (iVar1 != 0) {
            return func_02058b64((char *)iVar1);
        }
    }
    return 0;
}
