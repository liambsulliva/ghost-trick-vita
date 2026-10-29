extern char data_020aa08c;
extern int func_02008444();
extern int func_0200f70c();
extern int func_02010f0c();
extern int func_02010fbc();
extern int func_02011cdc();
extern int func_02013a14();
extern int func_02047120();

unsigned int * func_020493cc(unsigned int *param_1)

{
    int iVar1;
    unsigned char uVar2;
    unsigned int uVar3;
    unsigned int uVar4;

    func_02047120(param_1);
    *param_1 = ((unsigned int)&data_020aa08c);
    func_0200f70c(param_1 + 0x3c);
    uVar4 = 0;
    param_1[0x2e] = 0;
    param_1[0x31] = 0;
    do {
        iVar1 = uVar4 * 4;
        uVar4 = uVar4 + 1;
        *(unsigned int *)((int)param_1 + iVar1 + 0xbc) = 0;
    } while (uVar4 < 2);
    param_1[0x32] = 0;
    *(unsigned char *)(param_1 + 0x34) = 0;
    *(unsigned char *)((int)param_1 + 0xd1) = 0;
    *(unsigned char *)((int)param_1 + 0xd2) = 0;
    *(unsigned char *)((int)param_1 + 0xd3) = 0;
    *(unsigned char *)(param_1 + 0x3a) = 0;
    *(unsigned char *)((int)param_1 + 0xe9) = 0;
    *(unsigned char *)((int)param_1 + 0xea) = 0;
    param_1[0x33] = 0;
    func_02008444();
    uVar2 = func_02013a14();
    *(unsigned char *)((int)param_1 + 0xeb) = uVar2;
    uVar3 = func_02008444();
    uVar2 = func_02010fbc(uVar3,1);
    *(unsigned char *)(param_1 + 0x3b) = uVar2;
    if (*(char *)((int)param_1 + 0xeb) != '\0') {
        uVar3 = func_02008444();
        func_02011cdc(uVar3,0);
        if (*(char *)(param_1 + 0x3b) != '\0') {
            uVar3 = func_02008444();
            func_02010f0c(uVar3,1,0);
        }
    }
    return param_1;
}
