extern int func_0200f70c();
extern int func_02011774();

unsigned int * func_02057b54(unsigned int *param_1)

{
    unsigned int *puVar1;

    func_02011774();
    *param_1 = ((unsigned int)0x020ae110u);
    param_1[0x38] = 0;
    *(unsigned char *)((int)param_1 + 0xe6) = 0;
    *(unsigned char *)((int)param_1 + 0xe7) = 0;
    puVar1 = param_1 + 0x3a;
    do {
        func_0200f70c(puVar1);
        puVar1 = puVar1 + 3;
    } while (puVar1 < param_1 + 0x46);
    return param_1;
}
