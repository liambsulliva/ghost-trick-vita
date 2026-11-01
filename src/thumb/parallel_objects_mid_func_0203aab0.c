typedef void code();
void func_0203aab0(int *param_1)
{
    int iVar1;
    int iVar2;
    int *piVar3;
    iVar1 = *(int *)((char *)param_1 + 0xc8);
    iVar2 = param_1[10];
    if (iVar2 < iVar1 + 0x5a000) {
        param_1[10] = iVar2 + *(int *)((char *)param_1 + 0xec);
    } else {
        param_1[10] = iVar1 + 0xfa000;
        *(unsigned char *)((char *)param_1 + 0x10) = 5;
    }
    piVar3 = (int *)*(int *)((char *)param_1 + 0xf8);
    ((code *)((int *)*piVar3)[14])(piVar3, param_1[10]);
}
