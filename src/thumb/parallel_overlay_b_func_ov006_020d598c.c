extern int func_0201123c();
extern int func_02011264();
extern int func_02022f74();
extern int func_020521d4();
extern int func_ov006_020d4098();
extern int func_ov006_020d5760();
extern unsigned char data_020cdf78[];
extern unsigned char data_ov006_020d8de8[];

void func_ov006_020d598c(int param_1, unsigned int param_2)
{
    unsigned int uVar1;
    int iVar2;
    unsigned char uVar3;
    unsigned int uVar4;
    unsigned int uVar5;

    uVar1 = func_02011264();
    uVar4 = 0;
    uVar1 = func_0201123c(uVar1, 0);
    iVar2 = func_02022f74(uVar1, data_020cdf78);
    if (iVar2 == 0) {
        uVar3 = *(unsigned char *)(param_1 + 0x25) & 1;
        iVar2 = *(signed char *)(param_1 + uVar3 * 0x32 + *(signed char *)(param_1 + uVar3 + 0x32) + 0x38);
        if (uVar3 != 0 && *(unsigned char *)(param_1 + iVar2 + 0x3bc) == 0) {
            uVar4 = 1;
        }
        func_ov006_020d4098(param_2, uVar3, iVar2, uVar4, *(unsigned int *)(param_1 + uVar3 * 200 + iVar2 * 4 + 0x9c));
        *(unsigned char *)(param_1 + 0x24) = 7;
        uVar5 = 6;
        uVar5 = uVar5 + 0xfa;
        func_020521d4(data_ov006_020d8de8, 0, 6, uVar5);
        func_020521d4(data_ov006_020d8de8, 1, 6, uVar5);
        func_ov006_020d5760(1);
    }
}
