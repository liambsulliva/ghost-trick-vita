extern int func_020266f4();
extern int func_02027f9c();

void func_0205935c(unsigned char *param_1,int param_2)

{
    int iVar1;
    unsigned int uVar2;
    unsigned int uVar3;

    uVar3 = 0;
    do {
        iVar1 = uVar3 * 0xc;
        *(int *)(param_1 + iVar1 + 0xe4) = -*(int *)(param_1 + iVar1 + 0xe4);
        uVar3 = uVar3 + 1;
    } while (uVar3 < 4);
    for (uVar3 = 0; uVar2 = func_020266f4((int)param_1), uVar3 < uVar2; uVar3 = uVar3 + 1) {
        *(int *)(*(int *)(param_1 + uVar3 * 4 + 0xc4) + 8) =
                  -*(int *)(*(int *)(param_1 + uVar3 * 4 + 0xc4) + 8);
    }
    func_02027f9c(param_1,param_2);
    return;
}
