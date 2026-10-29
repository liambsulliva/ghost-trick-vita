extern char data_020a9a40;
extern char data_020ceccc;
extern int func_02047120();
extern int func_0204ed08();

unsigned int * func_0204af5c(unsigned int *param_1)

{
    unsigned int uVar1;

    func_02047120();
    *param_1 = ((unsigned int)&data_020a9a40);
    param_1[0x2e] = 0;
    param_1[0x2f] = 0;
    param_1[0x30] = 0xffffffff;
    param_1[0x31] = 0;
    *(unsigned char *)(param_1 + 0x32) = 0;
    uVar1 = func_0204ed08(((unsigned int)&data_020ceccc));
    param_1[0x2e] = uVar1;
    return param_1;
}
