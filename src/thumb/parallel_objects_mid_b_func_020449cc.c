extern void func_02010b74();
extern void func_02044d7c();

void func_020449cc(unsigned char *param_1)
{
    param_1[0xe0] = param_1[0xe0] | 4;
    if ((((unsigned int)param_1[0xe0]) << 30) >> 31 == 0) {
        if (*(int *)(param_1 + 0xdc) == 0) {
            func_02010b74(param_1);
            return;
        }
        func_02044d7c(param_1, 2, *(unsigned int *)(param_1 + 0x104) >> 1);
    }
}
