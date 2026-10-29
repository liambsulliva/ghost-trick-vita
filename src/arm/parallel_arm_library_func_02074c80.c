extern int func_02074aac();
extern int func_02074ac0();
extern int func_02074c6c();

unsigned int func_02074c80(int param_1,int param_2,int param_3)

{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned int uVar3;

    uVar1 = func_02074ac0(param_1);
    if (uVar1 == 0) {
        return 0;
    }
    uVar2 = ~(param_3 - 1U) & (uVar1 + param_3) - 1;
    uVar3 = ~(param_3 - 1U) & (unsigned int)((unsigned char *)(uVar2 + param_2) + param_3 - 1);
    uVar1 = func_02074aac(param_1);
    if (uVar3 > uVar1) {
        return 0;
    }
    func_02074c6c(param_1,uVar3);
    return uVar2;
}
