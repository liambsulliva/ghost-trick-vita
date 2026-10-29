extern char data_020aa328;
extern int func_0200f70c();
extern int func_02011774();

unsigned int * func_02047720(unsigned int *param_1)

{
    unsigned int *puVar1;

    func_02011774();
    *param_1 = ((unsigned int)&data_020aa328);
    param_1[0x36] = 0;
    param_1[0x37] = 0;
    *(unsigned char *)((int)param_1 + 0xe2) = 0;
    *(unsigned char *)((int)param_1 + 0xe3) = 0;
    puVar1 = param_1 + 0x39;
    do {
        func_0200f70c(puVar1);
        puVar1 = puVar1 + 3;
    } while (puVar1 < param_1 + 0x45);
    param_1[0x45] = 0;
    return param_1;
}
