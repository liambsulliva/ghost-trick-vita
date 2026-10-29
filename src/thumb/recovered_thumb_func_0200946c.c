extern int func_0200902c();
extern int func_020094b8();
extern int func_02009630();

void func_0200946c(int param_1)

{
    int iVar1;
    int iVar2;

    iVar1 = func_0200902c();
    iVar2 = 1;
    if (iVar1 == 1) {
        iVar2 = func_020094b8(param_1);
    }
    *(unsigned char *)(param_1 + 9) = 0;
    if (iVar1 != 1) {
        iVar2 = 1;
    }
    if (iVar2 == 0) {
        func_02009630(param_1,0,param_1 + ((int)0x000014e0u));
        *(unsigned char *)(param_1 + 6) = 1;
        return;
    }
    if (iVar1 == 2) {
        *(unsigned char *)(param_1 + 6) = 2;
        return;
    }
    *(unsigned char *)(param_1 + 6) = 2;
    return;
}
