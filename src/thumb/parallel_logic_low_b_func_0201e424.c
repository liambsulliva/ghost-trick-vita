struct SpliceList {
    int f00;
    int f04;
    unsigned int f08;
    int f0c;
    int f10;
    int f14;
    int f18;
    int f1c;
};

void func_0201e424(struct SpliceList *param_1, int param_2)
{
    unsigned int uVar1;
    int iVar2;
    if (param_1->f0c != 0 && param_2 != 0) {
        uVar1 = param_1->f08 & ~3u;
        *(int *)(param_2 + uVar1 - 4) = *(int *)(param_1->f14 + uVar1 - 4);
        *(int *)(param_1->f14 + (param_1->f08 & ~3u) - 4) = param_2;
        iVar2 = param_1->f10;
        param_1->f14 = param_2;
        if (iVar2 == 0) {
            param_1->f10 = param_2;
        }
        param_1->f1c = param_1->f1c - 1;
    }
}
