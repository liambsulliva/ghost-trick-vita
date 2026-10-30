typedef void code(int, int);

void func_0207d954(int *param_1)
{
    int iVar1;
    code *pfVar2;

    iVar1 = *param_1;
    if (iVar1 != 0) {
        pfVar2 = (code *)param_1[2];
        if (pfVar2 != 0) {
            pfVar2(iVar1, -1);
        }
    }
}
