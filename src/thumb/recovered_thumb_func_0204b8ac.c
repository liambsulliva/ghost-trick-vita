extern int func_0200f70c();
extern int func_02011774();

unsigned int * func_0204b8ac(unsigned int *param_1)

{
    unsigned int *puVar1;

    func_02011774();
    *param_1 = ((unsigned int)0x020aa1a0u);
    param_1[0x36] = 0;
    *(unsigned char *)((int)param_1 + 0xde) = 0;
    *(unsigned char *)((int)param_1 + 0xdf) = 0;
    *(unsigned short *)(param_1 + 0x38) = 0;
    puVar1 = param_1 + 0x39;
    do {
        func_0200f70c(puVar1);
        puVar1 = puVar1 + 3;
    } while (puVar1 < param_1 + 0x45);
    return param_1;
}
