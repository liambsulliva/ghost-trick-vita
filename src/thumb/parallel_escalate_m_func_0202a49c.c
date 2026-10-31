extern int func_0202a4f4();
extern int func_0202a4e8();
extern int func_0202a564();
extern int func_0202a60c();

void func_0202a49c(unsigned char *param_1,unsigned int param_2,unsigned int param_3)

{
    int iVar1;
    unsigned int uVar2;

    if (*(unsigned char *)(param_1 + 0x24) == 2) {
        return;
    }
    uVar2 = param_2 << 4;
    if (param_3 == *(unsigned short *)(param_1 + uVar2 + 0x3a)) {
        return;
    }
    if (func_0202a4f4(param_1,param_2,param_3) == 0) {
        return;
    }
    iVar1 = func_0202a4e8(param_1,param_3);
    if (func_0202a564(param_1,iVar1) == 0) {
        func_0202a60c(param_1,param_3);
        return;
    }
    iVar1 = *(int *)(param_1 + 0x30);
    *(int *)(param_1 + 0x30) = iVar1 + 1;
    *(int *)((int)param_1 + uVar2 + 0x3c) = iVar1;
}
