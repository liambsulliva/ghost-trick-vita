extern int func_0206c138(int *);
extern int func_0206e2a8(int *);
extern int func_0206c1c8(int *);

unsigned int func_0206e214(int *param_1)
{
    unsigned int uVar3;
    int *iVar4;
    unsigned int bVar0;
    int bVar1;
    int bVar2;

    uVar3 = 0;
    bVar0 = (param_1[5] & 2) != 0;
    if (bVar0 != 0) {
        iVar4 = (int *)param_1[8];
        bVar1 = func_0206c138(param_1);
        bVar2 = func_0206e2a8(param_1);
        if (bVar2 != 0) {
            param_1[5] = param_1[5] & ~4;
            uVar3 = iVar4[7];
            iVar4[1] = iVar4[5];
            iVar4[3] = iVar4[6];
            iVar4[7] = 0;
        }
        if (bVar1 != 0) {
            func_0206c1c8(param_1);
        }
    }
    return uVar3;
}
