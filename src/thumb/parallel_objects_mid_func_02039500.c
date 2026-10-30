extern int func_020379f8();
extern int func_0203971c();
void func_02039500(unsigned char *param_1)
{
    unsigned char *puVar1;

    puVar1 = param_1 + 0xe2;
    if (1 < *puVar1) {
        puVar1 = param_1 + 0xe2;
        *puVar1 = *puVar1 - 1;
    }
    func_0203971c(param_1, 0);
    func_020379f8((int)param_1);
    return;
}
