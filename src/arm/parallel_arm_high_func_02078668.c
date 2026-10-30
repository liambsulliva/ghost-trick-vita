extern int func_02078410(int a, int b, unsigned short *c, unsigned int d);

int func_02078668(unsigned int *param_1, unsigned int *param_2, unsigned int param_3, unsigned int param_4)
{
    int iVar1;
    unsigned short flags;

    iVar1 = func_02078410(15, 3, &flags, param_4);
    if (iVar1 != 0) {
        return iVar1;
    }
    if (param_1 != 0) {
        *param_1 = (flags & 8) != 0;
    }
    if (param_2 == 0) {
        return iVar1;
    }
    *param_2 = (flags & 4) != 0;
    return iVar1;
}
