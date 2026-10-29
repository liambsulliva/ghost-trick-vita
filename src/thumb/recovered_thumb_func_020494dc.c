extern char data_020aa08c;
extern int func_02008444();
extern int func_02010d18();
extern int func_02010f0c();
extern int func_02011cdc();
extern int func_0202f644();
extern int func_02047198();

unsigned int * func_020494dc(unsigned int *param_1)

{
    unsigned int uVar1;

    *param_1 = ((unsigned int)&data_020aa08c);
    if (*(char *)(param_1 + 0x3a) != '\0') {
        func_02008444();
        func_02010d18();
    }
    if (*(char *)((int)param_1 + 0xeb) != '\0') {
        uVar1 = func_02008444();
        func_02011cdc(uVar1,1);
        if (*(char *)(param_1 + 0x3b) != '\0') {
            uVar1 = func_02008444();
            func_02010f0c(uVar1,1,1);
        }
    }
    func_02047198(param_1);
    func_0202f644(param_1);
    return param_1;
}
