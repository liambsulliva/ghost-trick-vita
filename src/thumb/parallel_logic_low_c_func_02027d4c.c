extern int func_020790c8();
extern char data_020ca01c[];
extern char data_020ca02c[];
extern char data_020ca03c[];

int func_02027d4c(unsigned int param_1, int param_2)
{
    int iVar1;
    int iVar2;
    unsigned int uVar3;

    iVar2 = param_2 * 4;
    if (*(int *)(data_020ca03c + iVar2) == 0) {
        return 0;
    }
    if (*(int *)(data_020ca01c + iVar2) == 0) {
        return 0;
    }
    if (*(int *)(data_020ca02c + iVar2) == 0) {
        return 0;
    }
    for (uVar3 = 0; uVar3 < *(unsigned int *)(data_020ca01c + iVar2); uVar3 = uVar3 + 1) {
        iVar1 = func_020790c8(*(unsigned int *)(*(int *)(data_020ca02c + iVar2) + uVar3 * 8), param_1);
        if (iVar1 == 0) {
            return *(int *)(data_020ca03c + iVar2) + *(int *)(*(int *)(data_020ca02c + iVar2) + uVar3 * 8 + 4);
        }
    }
    return 0;
}
